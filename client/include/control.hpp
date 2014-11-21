#ifndef _GTKMM_CONTROL_HPP_
#define _GTKMM_CONTROL_HPP_

#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <gtkmm.h>

#include "movie_item.hpp"

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
    static const std::size_t COLUMN_ITEMS = 10;

    inline void hide_current_widgets();
    inline void show_current_widgets();
};
}

#endif
