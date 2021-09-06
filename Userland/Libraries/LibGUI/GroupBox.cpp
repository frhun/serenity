/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *               2021,      Frhun <frhun@t-online.de>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <AK/Utf8View.h>
#include <LibGUI/GroupBox.h>
#include <LibGUI/Painter.h>
#include <LibGfx/Font.h>
#include <LibGfx/Palette.h>
#include <LibGfx/StylePainter.h>

REGISTER_WIDGET(GUI, GroupBox)

namespace GUI {

GroupBox::GroupBox(const StringView& title)
    : m_title(title)
{
    REGISTER_STRING_PROPERTY("title", title, set_title);
    REGISTER_BOOL_PROPERTY("collapsible", is_collapsible, set_collapsible);
    REGISTER_BOOL_PROPERTY("collapsed", is_collapsed, set_collapsible);
}

GroupBox::~GroupBox()
{
}

const int extend_button_padding = 4;
const int extend_button_size = 18;

void GroupBox::paint_event(PaintEvent& event)
{
    const int frame_width = 2;
    const int extend_button_left_margin = 6;
    Painter painter(*this);
    painter.add_clip_rect(event.rect());

    String extend_button_text;
    if(is_collapsed())
        extend_button_text = AK::Utf8View{"+"}.as_string();
    else
        extend_button_text = AK::Utf8View{"-"}.as_string();

    Gfx::IntRect extend_button_rect {
            extend_button_left_margin, 0,
            AK::max(extend_button_size, font().width(extend_button_text) + extend_button_padding * 2),
            AK::max(extend_button_size, font().glyph_height() + extend_button_padding * 2)
        };

    int title_height = (!m_title.is_empty() ? font().glyph_height() : 0);
    int offset_due_to_title_and_collapse_button =
        AK::max(
                ( is_collapsible() || is_collapsed() ? extend_button_rect.height() : 0),
                title_height
            ) / 2;

    Gfx::IntRect frame_rect {
        0,
        offset_due_to_title_and_collapse_button - frame_width,
        width(),
        (is_collapsed() ? frame_width * 2 : height() + frame_width - offset_due_to_title_and_collapse_button)
    };
    Gfx::StylePainter::paint_frame(painter, frame_rect, palette(), Gfx::FrameShape::Box, Gfx::FrameShadow::Sunken, frame_width);

    int horizontal_title_offset = 0;
    if(is_collapsible() || is_collapsed()){
        horizontal_title_offset = extend_button_rect.x() + extend_button_rect.width();
        Gfx::StylePainter::paint_button(
                painter,
                extend_button_rect,
                palette(),
                Gfx::ButtonStyle::Normal,
                m_being_pressed && (m_hovered || m_being_keyboard_pressed),
                m_hovered,
                false /*checked*/,
                (is_collapsible() || is_collapsed()) && is_enabled(),
                is_focused()
            );
        painter.draw_text(extend_button_rect, extend_button_text, Gfx::TextAlignment::Center, palette().button_text());
    }

    if (!m_title.is_empty()) {
        Gfx::IntRect text_rect {
                horizontal_title_offset + 6,
                offset_due_to_title_and_collapse_button - (font().glyph_height() / 2),
                font().width(m_title) + 6,
                font().glyph_height()
            };
        painter.fill_rect(text_rect, palette().button());
        painter.draw_text(text_rect, m_title, Gfx::TextAlignment::Center, palette().button_text());
        if(is_focused()){
            auto focus_rect = text_rect;
            focus_rect.inflate(2, 6);
            painter.draw_focus_rect(focus_rect, palette().focus_outline());
        }
    }
}

void GroupBox::toggle_collapsed()
{
    set_collapsed(!is_collapsed());
}

Gfx::IntRect GroupBox::clickable_rect() const
{
    return Gfx::IntRect { 0, 0, width(), AK::max(extend_button_size, font().glyph_height() + extend_button_padding) };
}

void GroupBox::enter_event(Core::Event&)
{
    //m_hovered = true;
    update();
}

void GroupBox::leave_event(Core::Event&)
{
    m_hovered = false;
    update();
}

void GroupBox::mousemove_event(MouseEvent& event)
{
    bool is_over = clickable_rect().contains(event.position());

    if( m_hovered != is_over){
        m_hovered = is_over;
        if(m_being_pressed && !m_being_keyboard_pressed && !(event.buttons() & MouseButton::Left))
            m_being_pressed = false;
        update();
    }

    Widget::mousemove_event(event);
}


void GroupBox::mousedown_event(MouseEvent& event)
{
    if (    is_collapsible()
        && event.button() == MouseButton::Left
        && clickable_rect().contains(event.position())) {
        m_being_pressed = true;
        update();
    }
    Widget::mousedown_event(event);
}

void GroupBox::mouseup_event(GUI::MouseEvent& event)
{
    if(    is_collapsible()
        && event.button() == MouseButton::Left
        && clickable_rect().contains(event.position())
        && m_being_pressed
        ) {
        toggle_collapsed();
        m_being_pressed = false;
        update();
    }

    Widget::mouseup_event(event);
}

void GroupBox::keydown_event(KeyEvent& event)
{
    if (event.key() == KeyCode::Key_Return || event.key() == KeyCode::Key_Space) {
        m_being_pressed = true;
        m_being_keyboard_pressed = true;
        update();
        event.accept();
        return;
    } else if (m_being_pressed && event.key() == KeyCode::Key_Escape) {
        m_being_pressed = false;
        m_being_keyboard_pressed = false;
        update();
        event.accept();
        return;
    }
    Widget::keydown_event(event);
}

void GroupBox::keyup_event(KeyEvent& event)
{
    bool was_being_pressed = m_being_pressed;
    if ((event.key() == KeyCode::Key_Return || event.key() == KeyCode::Key_Space)){
        m_being_pressed = false;
        m_being_keyboard_pressed = false;
        if (was_being_pressed) {
            toggle_collapsed();
            update();
            event.accept();
            return;
        }
    }
    Widget::keyup_event(event);
}

void GroupBox::set_title(const StringView& title)
{
    if (m_title == title)
        return;
    m_title = title;
    update();
}

void GroupBox::set_collapsible(bool collapsible)
{
    if (m_collapsible == collapsible)
        return;
    set_focus_policy(collapsible ? GUI::FocusPolicy::StrongFocus : GUI::FocusPolicy::NoFocus);
    m_collapsible = collapsible;
    update();
}

void GroupBox::set_collapsed(bool collapsed)
{
    if (m_collapsed == collapsed)
        return;
    m_collapsed = collapsed;
    invalidate_layout();
    update();
}

Gfx::IntSize GroupBox::max_size() const
{
    if(is_collapsed())
        return Gfx::IntSize{Widget::max_size().width(), AK::max(extend_button_size, font().glyph_height() + extend_button_padding * 2)};
    else
        return Widget::max_size();
}

}
