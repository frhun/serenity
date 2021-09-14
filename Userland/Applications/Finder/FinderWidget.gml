@GUI::Widget{
layout: @GUI::VerticalBoxLayout {
    margins: [6]
}
min_height: 300

    @GUI::GroupBox {
        title: "Input Domain"
        layout: @GUI::VerticalBoxLayout {
            spacing: 4
            margins: [4]
        }
        
        @GUI::Widget {
            layout: @GUI::HorizontalBoxLayout {}
            shrink_to_fit: true
            
            @GUI::Label {
                text: "Base Path/File:"
                text_alignment: "CenterRight"
                max_width: 100
            }

            @GUI::TextBox {
                name: "base_path_text_box"
            }

            @GUI::Button {
                text: "Choose"
                name: "base_path_choose_button"
                max_width: 80
            }
        }
        
        @GUI::HorizontalSeparator
        
        @GUI::CheckBox {
            text: "Recursive Search"
            tooltip: "Search the specified path recursively"
        }
        
        @GUI::GroupBox {
            layout: @GUI::HorizontalBoxLayout {
                margins: [4]
                spacing: 4
            }
            title: "Path Filters"
            collapsible: true
            
            @GUI::ListView {
                name: "filter_list"
            }
            
            @GUI::Widget {
                layout: @GUI::VerticalBoxLayout {
                    margins: [0]
                    spacing: 4
                }
                max_width: 88
                
                @GUI::Button {
                    name: "exclude_add_button"
                    text: "Add"
                }
                
                @GUI::Button {
                    name: "exclude_remove_button"
                    text: "Remove"
                }
                
                @GUI::Button {
                    name: "exclude_edit_button"
                    text: "Edit"
                }
            }
        }
        
        @GUI::Widget {
            max_height: 0
        }
    }

    @GUI::GroupBox {
        title: "Search Pattern"
        layout: @GUI::VerticalBoxLayout {
            spacing: 4
            margins: [4]
        }
        shrink_to_fit: true
        
        @GUI::TextBox {
            name: "search_pattern_text_box"
        }
    }

    @GUI::Widget {
        layout: @GUI::HorizontalBoxLayout {
        }
        shrink_to_fit: true
        
        @GUI::Widget {}

        @GUI::Button {
            text: "Search"
            max_width: 120
        }
}
}
