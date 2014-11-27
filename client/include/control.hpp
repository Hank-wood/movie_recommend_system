#ifndef _GTKMM_CONTROL_HPP_
#define _GTKMM_CONTROL_HPP_

#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <cstring>
#include <gtkmm.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "movie_item.hpp"
#include "../../common/package.hpp"
#include "../../common/movie.hpp"

namespace Custom {
class Control {
public:
    Control(Gtk::Box *left, Gtk::Box *right);
    Control(const Control&) = delete;
    Control& operator=(const Control&) = delete;

    void next_page();
    void prev_page();
    void connect();
    void submit();

private:
    Gtk::Box *box_left, *box_right;
    std::map<std::size_t, std::vector<movie_item*>> items;
    std::size_t cur_page = 0;
    static const std::size_t COLUMN_ITEMS = 16;
    static const std::size_t BUFSIZE = 256;

    int sockfd = 0;
    char buf[BUFSIZE];

    inline void hide_current_widgets();
    inline void show_current_widgets();
};
}

#endif
