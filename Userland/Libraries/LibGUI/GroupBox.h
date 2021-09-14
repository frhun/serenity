/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *               2021,      Frhun <frhun@t-online.de>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibGUI/Widget.h>

namespace GUI {

class GroupBox : public Widget {
    C_OBJECT(GroupBox)
public:
    virtual ~GroupBox() override;

    String title() const { return m_title; }
    void set_title(const StringView&);
    bool is_collapsible() const { return m_collapsible; }
    void set_collapsible(bool);
    bool is_collapsed() const { return m_collapsed; }
    void set_collapsed(bool);
    void toggle_collapsed();
    virtual Gfx::IntSize max_size() const override;
    virtual Margins content_margins() const override;

protected:
    explicit GroupBox(const StringView& title = {});

    virtual void paint_event(PaintEvent&) override;

private:
    virtual void leave_event(Core::Event&) override;
    virtual void focusout_event(GUI::FocusEvent&) override;
    virtual void mouseup_event(GUI::MouseEvent&) override;
    virtual void mousedown_event(GUI::MouseEvent&) override;
    virtual void mousemove_event(GUI::MouseEvent&) override;
    virtual void keydown_event(GUI::KeyEvent&) override;
    virtual void keyup_event(GUI::KeyEvent&) override;
    virtual void fonts_change_event(FontsChangeEvent&) override;

    Gfx::IntRect clickable_rect() const;

    String m_title;
    bool m_collapsible { false };
    bool m_collapsed { false };
    bool m_being_pressed { false };
    bool m_being_keyboard_pressed { false };
    bool m_hovered { false };
};

}
