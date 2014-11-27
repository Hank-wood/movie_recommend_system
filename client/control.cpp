#include "include/control.hpp"
#include <iostream>

namespace Custom {
Control::Control(Gtk::Box *left, Gtk::Box *right)
    : box_left(left), box_right(right) {
}

void Control::next_page() {
#ifndef NDEBUG
    std::cout << "next_page pressed\n";
#endif
    if (cur_page != items.size() - 1) {
        hide_current_widgets();
        ++cur_page;
        show_current_widgets();
    }
    std::cout << (cur_page + 1) << "/" << items.size() << std::endl;
}

void Control::prev_page() {
#ifndef NDEBUG
    std::cout << "prev_page pressed\n";
#endif
    if (cur_page != 0) {
        hide_current_widgets();
        --cur_page;
        show_current_widgets();
    }
    std::cout << (cur_page + 1) << "/" << items.size() << std::endl;
}

void Control::connect() {
#ifndef NDEBUG
    std::cout << "connect pressed\n";
#endif
    // establish connection and receive data
    struct addrinfo hints, *servinfo;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    const char *host = "127.0.0.1", *port = "8086";
    if (::getaddrinfo(host, port, &hints, &servinfo) != 0) {
        std::cerr << "Get address info error.\n";
        return;
    }

    if ((sockfd = ::socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
        std::cerr << "Create socket error.\n";
        return;
    }

    if (::connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        ::close(sockfd);
        return;
    }

    ::freeaddrinfo(servinfo);

    // clear the previous widgets.
    for (auto &item : items) {
        auto &vec = item.second;
        for (auto &i : vec)
            delete i;
    }
    items.clear();

    // receive widgets' information.
    Network::package pk = { Network::package::FETCH };
    Network::send(sockfd, pk);
    int32_t total = 0, cur = 0;
    Network::recv(sockfd, &total);
    while (total-- > 0) {
        Movie movie = {0};
        Network::recv(sockfd, &movie);
        if (items[cur].size() == 2 * COLUMN_ITEMS)
            ++cur;
        items[cur].push_back(new movie_item(movie.id, movie.name));
    }
    for (auto &item : items) {
        auto &vec = item.second;
        for (std::size_t i = 0; i < COLUMN_ITEMS && i < vec.size(); ++i)
            box_left->pack_start(*vec[i]);
        for (std::size_t i = COLUMN_ITEMS; i < vec.size(); ++i)
            box_right->pack_start(*vec[i]);
    }
    show_current_widgets();
}

void Control::submit() {
#ifndef NDEBUG
    std::cout << "submit pressed\n";
#endif
    if (sockfd == 0)
        return;

    int32_t total = std::accumulate(items.begin(), items.end(), 0,
                                    [](const auto &init, const auto &x) {
        return init +
               std::count_if(x.second.begin(), x.second.end(),
                             [](const auto &y) { return y->get_vote() != 0; });
    });

    Network::package pk = { Network::package::COMMIT };
    Network::send(sockfd, pk);

    Network::send(sockfd, total);
    for (auto &item : items) {
        auto &vec = item.second;
        for (auto &i : vec) {
            Movie movie = { 0 };
            movie.id = i->get_id();
            movie.vote = i->get_vote();

            if (movie.vote == 0)
                continue;
            Network::send(sockfd, movie);
        }
    }

    int32_t recommends = 0;
    Network::recv(sockfd, &recommends);
    std::cout << "====Recommended movies starts.\n";
    while (recommends-- > 0) {
        Movie movie = { 0 };
        Network::recv(sockfd, &movie);
        std::cout << '\t' << movie.name << " " << movie.vote << std::endl;
    }
    std::cout << "====Recommended movies ends.\n";
}

void Control::hide_current_widgets() {
    for (auto &item : items[cur_page])
        item->hide();
}

void Control::show_current_widgets() {
    for (auto &item : items[cur_page])
        item->show();
}
}
