#ifndef _VOTE_WIDGET_H_
#define _VOTE_WIDGET_H_

#include <memory>
#include <array>
#include <gtkmm/box.h>
#include <gtkmm/button.h>

class VoteWidget : public Gtk::HBox {
public:
    VoteWidget();
    virtual ~VoteWidget();

    std::size_t get_vote() const { return rating; }

protected:
    std::array<Gtk::Button, 6> star;
    std::size_t rating;

    void unload_star();
    void load_star(std::size_t);
};

#endif
