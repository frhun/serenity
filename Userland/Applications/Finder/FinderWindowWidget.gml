@GUI::Widget {
    layout: @GUI::VerticalBoxLayout {
        margins: [0]
    }
    spacing: 2
    fill_with_background_color: true
    
    @GUI::VerticalSplitter {
        name: "splitter"
    }
    
    @GUI::Statusbar {
        name: "statusbar"

        @GUI::Progressbar {
            name: "progressbar"
            text: "Searching: "
            visible: false
        }
    }
}
