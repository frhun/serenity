/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *               2021,      Frhun <frhun@t-online.de>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

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

// Constants that define the look of the widget, and are needed to calculate various values
const int extend_button_padding = 4;
const int extend_button_size = 18;
const int frame_width = 2;

void GroupBox::paint_event(PaintEvent& event)
{
    const int extend_button_left_margin = 6;
    Painter painter(*this);
    painter.add_clip_rect(event.rect());

    String extend_button_text;
    if (is_collapsed())
        extend_button_text = { "+" };
    else
        extend_button_text = { "-" };

    Gfx::IntRect extend_button_rect {
        extend_button_left_margin, 0,
        AK::max(extend_button_size, font().width(extend_button_text) + extend_button_padding * 2),
        AK::max(extend_button_size, font().glyph_height() + extend_button_padding * 2)
    };

    int offset_due_to_title_and_collapse_button = AK::max(
                                                      (is_collapsible() || is_collapsed() ? extend_button_rect.height() : 0), //Note: this assumes that the button is allways taller than the label, if this is ever not the case, this goes wrong
                                                      (!m_title.is_empty() ? font().glyph_height() + 2 /*room for the focus rect*/ : 0))
        / 2;
    int offset_due_to_title_and_collapse_button_for_frame = AK::max(offset_due_to_title_and_collapse_button - frame_width, 0);

    Gfx::IntRect frame_rect {
        0,
        offset_due_to_title_and_collapse_button_for_frame,
        width(),
        (is_collapsed() ? frame_width * 2 : height() - offset_due_to_title_and_collapse_button_for_frame)
    };
    Gfx::StylePainter::paint_frame(painter, frame_rect, palette(), Gfx::FrameShape::Box, Gfx::FrameShadow::Sunken, frame_width);

    bool visually_pressed = m_being_pressed && (m_hovered || m_being_keyboard_pressed);

    int horizontal_title_offset = 0;
    if (is_collapsible() || is_collapsed()) {
        horizontal_title_offset = extend_button_rect.x() + extend_button_rect.width();
        Gfx::StylePainter::paint_button(
            painter,
            extend_button_rect,
            palette(),
            Gfx::ButtonStyle::Normal,
            visually_pressed,
            m_hovered,
            false /*checked*/,
            (is_collapsible() || is_collapsed()) && is_enabled(),
            is_focused());
        painter.draw_text(
            (visually_pressed ? extend_button_rect.translated({ 2, 2 }).shrunken(1, 1) : extend_button_rect),
            extend_button_text,
            Gfx::TextAlignment::Center,
            palette().button_text());
    }

    if (!m_title.is_empty()) {
        Gfx::IntRect text_rect {
            horizontal_title_offset + 7,
            offset_due_to_title_and_collapse_button - (font().glyph_height() / 2),
            font().width(m_title) + 7,
            font().glyph_height()
        };
        auto backgroud_rect = text_rect.inflated(2, 2);
        painter.fill_rect(backgroud_rect, palette().button());
        painter.draw_text(text_rect, m_title, Gfx::TextAlignment::Center, palette().button_text());
        if (is_focused())
            painter.draw_focus_rect(backgroud_rect, palette().focus_outline());
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

void GroupBox::leave_event(Core::Event& event)
{
    m_hovered = false;
    if (m_being_keyboard_pressed) {
        m_being_keyboard_pressed = false;
        m_being_pressed = false;
    }
    update();
    event.accept();
    Widget::leave_event(event);
}

void GroupBox::focusout_event(GUI::FocusEvent& focus_event)
{
    if (m_being_keyboard_pressed) {
        m_being_pressed = false;
        m_being_keyboard_pressed = false;
        focus_event.accept();
        update();
    }
    Widget::focusout_event(focus_event);
}

void GroupBox::mousemove_event(GUI::MouseEvent& event)
{
    bool is_over = clickable_rect().contains(event.position());

    if (m_hovered != is_over) {
        m_hovered = is_over;
        if (m_being_pressed && !m_being_keyboard_pressed && !(event.buttons() & MouseButton::Left))
            m_being_pressed = false;
        event.accept();
        update();
    }

    Widget::mousemove_event(event);
}

void GroupBox::mousedown_event(GUI::MouseEvent& event)
{
    if (is_collapsible()
        && event.button() == MouseButton::Left
        && clickable_rect().contains(event.position())) {
        m_being_pressed = true;
        event.accept();
        update();
    }
    Widget::mousedown_event(event);
}

void GroupBox::mouseup_event(GUI::MouseEvent& event)
{
    if (is_collapsible()
        && event.button() == MouseButton::Left
        && clickable_rect().contains(event.position())
        && m_being_pressed) {
        toggle_collapsed();
        m_being_pressed = false;
        event.accept();
        update();
    }

    Widget::mouseup_event(event);
}

void GroupBox::keydown_event(GUI::KeyEvent& event)
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

void GroupBox::keyup_event(GUI::KeyEvent& event)
{
    bool was_being_pressed = m_being_pressed;
    if ((event.key() == KeyCode::Key_Return || event.key() == KeyCode::Key_Space)) {
        m_being_pressed = false;
        m_being_keyboard_pressed = false;
        if (was_being_pressed) {
            toggle_collapsed();
            event.accept();
            update();
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
    m_wants_children_ignored = collapsed;
    invalidate_layout();
    update();
}

Gfx::IntSize GroupBox::max_size() const
{
    if (is_collapsed()) //Note: this assumes that the button is allways taller than the label, if this is ever not the case, this goes wrong
        return Gfx::IntSize { Widget::max_size().width(), AK::max(extend_button_size, font().glyph_height() + extend_button_padding * 2) };
    else
        return Widget::max_size();
}

}
