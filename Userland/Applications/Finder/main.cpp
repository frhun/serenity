/*
 * Copyright (c) 2021, Frhun <frhun@t-online.de>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Entry point for the finder application.
 * The GUI counterpart to grep.
 */

#include "FinderWindowWidget.h"
#include <LibGUI/Action.h>
#include <LibGUI/Application.h>
#include <LibGUI/Icon.h>
#include <LibGUI/Menu.h>
#include <LibGUI/Menubar.h>
#include <LibGUI/Window.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (pledge("stdio recvfd sendfd rpath unix", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    auto app = GUI::Application::construct(argc, argv);

    if (pledge("stdio recvfd sendfd rpath", nullptr) < 0) {
        perror("pledge");
        return 1;
    }

    if (unveil("/res", "r") < 0) {
        perror("unveil");
        return 1;
    }

    unveil(nullptr, nullptr);

    auto app_icon = GUI::Icon::default_icon("app-finder");

    auto window = GUI::Window::construct();
    window->set_title("Finder");
    window->set_resizable(true);
    window->resize(400, 500);
    window->set_minimum_size(400, 400);

    window->set_main_widget<FinderWindowWidget>();

    window->set_icon(app_icon.bitmap_for_size(16));

    auto& file_menu = window->add_menu("&File");
    file_menu.add_action(GUI::CommonActions::make_quit_action([](auto&) {
        GUI::Application::the()->quit();
    }));

    auto& help_menu = window->add_menu("&Help");
    help_menu.add_action(GUI::CommonActions::make_about_action("Finder", app_icon, window));

    window->show();

    return app->exec();
}
