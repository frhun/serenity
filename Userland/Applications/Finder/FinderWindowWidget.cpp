/*
 * Copyright (c) 2021, Frhun <frhun@t-online.de>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibGUI/Splitter.h>

#include "FinderWindowWidget.h"
#include <Applications/Finder/FinderWindowWidgetGML.h>


FinderWindowWidget::FinderWindowWidget() {
    load_from_gml(finder_window_widget_gml);
    
    auto splitter = find_child_of_type_named<GUI::VerticalSplitter>("splitter");
    
    //m_result_list_view = find_child_of_type_named<GUI::ListView>("results_list");
    //m_finder_widget = FinderWidget::construct();
    //splitter->insert_child_before(*m_finder_widget, *m_result_list_view);
    
    m_finder_widget = splitter->add<FinderWidget>();
    m_result_list_view = splitter->add<GUI::ListView>();
}
