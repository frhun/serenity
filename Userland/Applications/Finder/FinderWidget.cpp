/*
 * Copyright (c) 2021, Frhun <frhun@t-online.de>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
 
#include "FinderWidget.h"
#include <Applications/Finder/FinderWidgetGML.h>
#include <LibGUI/Button.h>
#include <LibGUI/Label.h>
#include <LibGUI/TextBox.h>

FinderWidget::FinderWidget(){
    load_from_gml(finder_widget_gml);
}
