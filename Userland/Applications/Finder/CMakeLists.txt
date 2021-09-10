serenity_component(
    Finder
    REQUIRED
    TARGETS Finder
)

compile_gml(FinderWidget.gml FinderWidgetGML.h finder_widget_gml)
compile_gml(FinderWindowWidget.gml FinderWindowWidgetGML.h finder_window_widget_gml)

set(SOURCES
    FinderWidgetGML.h
    FinderWidget.gml
    FinderWidget.cpp
    FinderWidget.h
    FinderWindowWidget.gml
    FinderWindowWidgetGML.h
    FinderWindowWidget.cpp
    FinderWindowWidget.h
    main.cpp
)

serenity_app(Finder ICON app-finder)
target_link_libraries(Finder LibGUI)
