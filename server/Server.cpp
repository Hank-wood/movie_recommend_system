#include "include/Server.hpp"
#include "ml/pred.hpp"

Server::Server() {
    signal(SIGCHLD, SIG_IGN);

    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
#ifndef NDEBUG
        std::perror("socket");
#endif
        return;
    }

    const int reuse = 1;
    ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in sin;
    ::bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(PORT);
    if (::bind(fd, (const sockaddr*)&sin, sizeof(sin)) == -1) {
        fd = -1;
#ifndef NDEBUG
        std::perror("bind");
#endif
        return;
    }
    if (::listen(fd, 10) == -1) {
        fd = -1;
#ifndef NDEBUG
        std::perror("listen");
#endif
        return;
    }
}

Server::~Server() { ::close(fd); }

void Server::mainloop() {
    if (fd == -1)
        return;
    while (true) {
        struct sockaddr_in sin;
        socklen_t len = sizeof(sin);
        connfd = ::accept(fd, (struct sockaddr*)&sin, &len);
#ifndef NDEBUG
        std::cerr << "Connect from " << ::inet_ntoa(sin.sin_addr) << ", port = " << sin.sin_port << "\n";
#endif
        if (connfd == -1) {
#ifndef NDEBUG
            std::perror("accept");
#endif
            continue;
        }
        int pid = fork();
        if (pid == 0) {
            process();
            break;
        }
    }
}

void Server::process() {
    Network::package pk;
    while (true) {
        Network::recv(connfd, &pk);
        if (pk.tag == Network::package::FETCH)
            send_movie_list();
        else
            recv_movie_vote();
    }
}

void Server::send_movie_list() {
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    std::unique_ptr<sql::Connection> con(driver->connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD));
    con->setSchema(MYSQL_DATABASE);
    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("select count(*) from movies"));

    res->next();
    int32_t total = res->getInt(1);

    Network::send(connfd, total);
    res.reset(stmt->executeQuery("select * from movies"));
    while (res->next()) {
        Movie movie = { 0 };
        movie.id = res->getInt("movie_id");
        std::string _ = res->getString("movie_name");
        _.copy(movie.name, _.size());
        Network::send(connfd, movie);
    }
}

void Server::recv_movie_vote() {
    int32_t total = 0;
    std::vector<std::pair<int, int>> vec;
    Network::recv(connfd, &total);
    while (total-- > 0) {
        Movie movie = { 0 };
        Network::recv(connfd, &movie);
        vec.push_back(std::make_pair(movie.id, movie.vote));
    }

    // Get the count of movies and the count of users. 
    // Construct the taste matrix based on this.
    int movies_cnt = 0, users_cnt = 0;
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    std::unique_ptr<sql::Connection> con(driver->connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD));
    con->setSchema(MYSQL_DATABASE);
    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("select count(*) from movies"));
    res->next();
    movies_cnt = res->getInt(1);
    res.reset(stmt->executeQuery("select count(*) from users"));
    res->next();
    users_cnt = res->getInt(1);

    std::vector<int> taste(movies_cnt, 0);
    for (auto &i : vec)
        taste[i.first - 1] = i.second;

    std::vector<std::vector<int>> mtrx(users_cnt, std::vector<int>(movies_cnt));
    res.reset(stmt->executeQuery("select * from votes"));
    while (res->next()) {
        int uid = res->getInt("user_id"), mid = res->getInt("movie_id"),
            v = res->getInt("votes");
        mtrx[uid - 1][mid - 1] = v;
    }
    mtrx.push_back(taste);

    // Start to recommend
    const int32_t TOP = 10;
    std::vector<Movie> recommends = ML::predicate(mtrx, TOP);

    // Load movie list
    std::vector<std::string> movie_list(movies_cnt);
    res.reset(stmt->executeQuery("select * from movies"));
    while (res->next()) {
        int id = res->getInt("movie_id");
        std::string name = res->getString("movie_name");
        movie_list[id - 1] = name;
    }
    for (auto& i : recommends)
        std::strncpy(i.name, movie_list[i.id - 1].c_str(), sizeof(i.name));

    Network::send(connfd, static_cast<int32_t>(recommends.size()));
    for (auto &i : recommends)
        Network::send(connfd, i);
}

