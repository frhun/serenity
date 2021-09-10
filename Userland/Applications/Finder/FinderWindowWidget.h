/*
 * Copyright (c) 2021, Frhun <frhun@t-online.de>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * 
 * The main window of the stand-alone Finder applicaiton.
 */
 
#pragma once

#include <LibGUI/Widget.h>
#include <AK/RefPtr.h>
#include <AK/NonnullRefPtr.h>
#include <LibGUI/ListView.h>
#include "FinderWidget.h"

class FinderWindowWidget final : public GUI::Widget {
    C_OBJECT(FinderWindowWidget)
private:
    FinderWindowWidget();
    
    RefPtr<GUI::ListView> m_result_list_view;
    RefPtr<FinderWidget> m_finder_widget;
};
