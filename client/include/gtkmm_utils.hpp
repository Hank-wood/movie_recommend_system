#ifndef _GTKMM_UTILS_H_
#define _GTKMM_UTILS_H_

#include <gtkmm.h>
#include <utility>

namespace Custom {
template<typename T>
T* Get(Glib::RefPtr<Gtk::Builder>& ref, const Glib::ustring& name) {
    T *p = nullptr;
    ref->get_widget(name, p);
    return p;
}
}

#endif
