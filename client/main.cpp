#include <gtkmm.h>
#include <iostream>

#include "include/gtkmm_utils.hpp"
#include "include/control.hpp"

int main(int argc, char* argv[]) {
    Gtk::Main kit(argc, argv);

    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("basic.ui");
    } catch(const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    } catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    } catch(...) {
        std::cerr << "UnknownError.\n";
        return 1;
    }

    if (auto window = Custom::Get<Gtk::Window>(refBuilder, "window")) {
        Custom::Control ins(Custom::Get<Gtk::Box>(refBuilder, "box_left"),
                            Custom::Get<Gtk::Box>(refBuilder, "box_right"));
        auto con = Custom::Get<Gtk::Button>(refBuilder, "connect_server");
        auto prev = Custom::Get<Gtk::Button>(refBuilder, "prev_page");
        auto next = Custom::Get<Gtk::Button>(refBuilder, "next_page");
        auto sub = Custom::Get<Gtk::Button>(refBuilder, "submit");
        if (!con || !prev || !next || !sub)
            return 1;
        con->signal_clicked().connect(sigc::mem_fun(ins, &Custom::Control::connect));
        prev->signal_clicked().connect(sigc::mem_fun(ins, &Custom::Control::prev_page));
        next->signal_clicked().connect(sigc::mem_fun(ins, &Custom::Control::next_page));
        sub->signal_clicked().connect(sigc::mem_fun(ins, &Custom::Control::submit));
        kit.run(*window);
    }
    return 0;
}
