#include "include/control.hpp"
#include <iostream>

namespace Custom {
Control::Control(Gtk::Box *left, Gtk::Box *right)
    : box_left(left), box_right(right) {
    // TEST
    items[0] = {
        new movie_item(0, "a"), new movie_item(1, "b"), new movie_item(2, "c"),
        new movie_item(3, "d"), new movie_item(3, "d"), new movie_item(3, "d"),
        new movie_item(3, "d"), new movie_item(3, "d"), new movie_item(3, "d"),
        new movie_item(3, "d"), new movie_item(3, "d"), new movie_item(3, "d"),
        new movie_item(3, "d"), new movie_item(3, "d"), new movie_item(3, "d"),
        new movie_item(3, "d"), new movie_item(3, "d"), new movie_item(3, "d"),
        new movie_item(3, "d"), new movie_item(3, "d"),
    };
    items[1] = {
        new movie_item(0, "a"), new movie_item(1, "b"), new movie_item(2, "c"),
        new movie_item(3, "d"), new movie_item(3, "e"), new movie_item(3, "f"),
        new movie_item(3, "t"), new movie_item(3, "n"), new movie_item(3, "g"),
        new movie_item(3, "s"), new movie_item(3, "m"), new movie_item(3, "h"),
        new movie_item(3, "r"), new movie_item(3, "l"), new movie_item(3, "i"),
        new movie_item(3, "q"), new movie_item(3, "k"), new movie_item(3, "j"),
        new movie_item(3, "p"), new movie_item(3, "o"),
    };
    items[2] = {
        new movie_item(0, "a"), new movie_item(1, "b"), new movie_item(2, "c"),
        new movie_item(3, "d"), new movie_item(3, "d"), new movie_item(3, "d"),
        new movie_item(3, "d"), new movie_item(3, "d"), new movie_item(3, "d"),
        new movie_item(3, "d"), new movie_item(3, "d"), new movie_item(3, "d"),
        new movie_item(3, "d"), new movie_item(3, "d"), new movie_item(3, "d"),
        new movie_item(3, "d"), new movie_item(3, "d"), new movie_item(3, "d"),
    };

    for (auto &item : items) {
        auto &vec = item.second;
        for (std::size_t i = 0; i < COLUMN_ITEMS && i < vec.size(); ++i)
            box_left->pack_start(*vec[i]);
        for (std::size_t i = COLUMN_ITEMS; i < vec.size(); ++i)
            box_right->pack_start(*vec[i]);
    }
    show_current_widgets();
}

void Control::next_page() {
    std::cout << "next_page pressed\n";
    if (cur_page == items.size() - 1)
        return;
    hide_current_widgets();
    ++cur_page;
    show_current_widgets();
}

void Control::prev_page() {
    std::cout << "prev_page pressed\n";
    if (cur_page == 0)
        return;
    hide_current_widgets();
    --cur_page;
    show_current_widgets();
}

void Control::connect() {
    std::cout << "connect pressed\n";
}

void Control::submit() {
    std::cout << "submit pressed\n";
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
