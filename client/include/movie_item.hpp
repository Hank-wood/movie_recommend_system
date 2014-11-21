#ifndef _MOVIE_ITEM_H_
#define _MOVIE_ITEM_H_

#include <memory>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <glibmm/ustring.h>

#include "votewidget.hpp"

class movie_item : public Gtk::HBox {
public:
    movie_item(std::size_t id, const Glib::ustring &name)
        : id(id), movie_name(new Gtk::Label(name)), vote(new VoteWidget) {
        pack_start(*movie_name);
        pack_start(*vote);
        movie_name->show();
        vote->show();
    }

    std::size_t get_vote() const { return vote->get_vote(); }
    std::size_t get_id() const { return id; }

protected:
    std::size_t id;
    std::shared_ptr<Gtk::Label> movie_name;
    std::shared_ptr<VoteWidget> vote;
};

#endif
