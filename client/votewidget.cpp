#include "include/votewidget.hpp"

VoteWidget::VoteWidget()
    : Gtk::HBox(true, 2), adj(Gtk::Adjustment::create(0, 0, 5, 1, 1)),
      scale_digits(adj) {
    pack_start(scale_digits);
    scale_digits.set_digits(0);
    show_all_children();
}

VoteWidget::~VoteWidget() {}
