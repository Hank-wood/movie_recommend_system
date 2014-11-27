#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
#include <string>
#include <memory>
#include <cstdio>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>

#define MYSQL_HOST "localhost:3306"
#define MYSQL_USER "root"
#define MYSQL_PASSWD "rootme"
#define MYSQL_DATABASE "recommend_sys"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <signal.h>

#include "../../common/package.hpp"
#include "../../common/movie.hpp"

class Server {
public:
    Server();
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    ~Server();
    void mainloop();

private:
    int fd;
    int connfd;
    static const int PORT = 8086;

    void process();
    void send_movie_list();
    void recv_movie_vote();
};

#endif
