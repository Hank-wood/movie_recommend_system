#ifndef _VOTE_WIDGET_H_
#define _VOTE_WIDGET_H_

#include <gtkmm.h>
#include <cstddef>

class VoteWidget : public Gtk::HBox {
public:
    VoteWidget();
    virtual ~VoteWidget();
    std::size_t get_vote() const { return adj->get_value(); }

protected:
    void on_adjustment_value_changed();
    Glib::RefPtr<Gtk::Adjustment> adj;
    Gtk::HScale scale_digits;
};

#endif
