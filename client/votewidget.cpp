#include "include/votewidget.hpp"

VoteWidget::VoteWidget() : Gtk::HBox(true, 0), rating(0) {
    for (std::size_t i = 0; i < star.size(); ++i) {
        pack_start(star[i]);
        star[i].signal_clicked().connect(
            sigc::bind(sigc::mem_fun(*this, &VoteWidget::load_star), i));
        star[i].show();
    }
    load_star(0);
    show();
}

VoteWidget::~VoteWidget() {}

void VoteWidget::load_star(std::size_t to) {
    unload_star();
    for (std::size_t i = 0; i <= to; ++i)
        star[i].set_image_from_icon_name("gtk-yes.png", Gtk::ICON_SIZE_BUTTON, true);
    rating = to;
}

void VoteWidget::unload_star() {
    for (auto &i : star)
        i.set_image_from_icon_name("");
}
