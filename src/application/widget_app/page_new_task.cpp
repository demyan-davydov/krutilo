// -*- coding:utf-8-unix -*-
/*!
    @file       page_new_task.cpp
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      'New Task' wizard page
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/


#include "page_new_task.h"
#include "widget_header.h"
#include "input_time.h"
#include "input_number.h"
#include "input_select.h"
#include "smooth_label.h"

#include "labels_win1251.h"

#include "../application.h"
#include "../application_keys.h"


PageNewTask::PageNewTask(Widget* parent, Task& task, Widget::State const& initState)
: base_t(parent, initState)
, m_task(task)
, m_background(make_565_color(0,64,20))
{

}

void PageNewTask::onInit(GContext const& ctx)
{
    constexpr int gap1 = 16;
    constexpr int gap2 = 10;
    constexpr int input_w = 136;
    constexpr int k_label_gap = 10;
    constexpr int k_label_esc_gap = -43;
    constexpr int k_label_mark_gap = -60;

    append<WidgetHeader>(GCExtents2d{{70,0}, {9,13}}, label_new_task);

    GCPoint pt = {85,69}; // start point for child widgets

    m_input_rpm = &append<Select>(ctx, pt, input_w); pt.y += m_input_rpm->rect().height() + gap1;
    m_input_time = &append<InputTime>(ctx, pt, input_w); pt.y += m_input_time->rect().height() + gap2;
    m_input_rev = &append<InputNumber>(ctx, pt, input_w); 

    const GFXfont& font_labels = uistyle().smallFont();
    m_label_rpm  = &append<SmoothLabel>(m_input_rpm->rect().midRight() + GCPoint_X(k_label_gap), Anchor::MidLeft, label_speed, font_labels);
    m_label_time = &append<SmoothLabel>(m_input_time->rect().midRight() + GCPoint_X(k_label_gap), Anchor::MidLeft, label_time, font_labels);
    m_label_rev  = &append<SmoothLabel>(m_input_rev->rect().midRight() + GCPoint_X(k_label_gap), Anchor::MidLeft, label_revolutions, font_labels);

    m_label_rpm->setVisible(false, false);
    m_label_time->setVisible(false, false);
    m_label_rev->setVisible(false, false);

    m_label_rpm->color(Clr_LIGHT_SLATE_GRAY);
    m_label_time->color(Clr_LIGHT_SLATE_GRAY);
    m_label_rev->color(Clr_LIGHT_SLATE_GRAY);

    const GFXfont& font_marks = uistyle().normalFont();
    m_label_esc_rpm  = &append<SmoothLabel>(m_input_rpm->rect().midLeft() + GCPoint_X(k_label_esc_gap), Anchor::MidRight, label_cancel, font_marks);
    m_label_esc_time = &append<SmoothLabel>(m_input_time->rect().midLeft() + GCPoint_X(k_label_esc_gap), Anchor::MidRight, label_cancel, font_marks);
    m_label_esc_rev  = &append<SmoothLabel>(m_input_rev->rect().midLeft() + GCPoint_X(k_label_esc_gap), Anchor::MidRight, label_cancel, font_marks);
    m_label_mark_time = &append<SmoothLabel>(m_input_time->rect().midLeft() + GCPoint_X(k_label_mark_gap), Anchor::MidRight, label_check, font_marks);
    m_label_mark_rev  = &append<SmoothLabel>(m_input_rev->rect().midLeft() + GCPoint_X(k_label_mark_gap), Anchor::MidRight, label_check, font_marks);

    m_label_esc_rpm->setVisible(false, false);
    m_label_esc_time->setVisible(false, false);
    m_label_esc_rev->setVisible(false, false);
    m_label_mark_time->setVisible(false, false);
    m_label_mark_rev->setVisible(false, false);

    m_label_esc_rpm->color(Clr_YELLOW);
    m_label_esc_time->color(Clr_YELLOW);
    m_label_esc_rev->color(Clr_YELLOW);
    m_label_mark_time->color(Clr_GREEN);
    m_label_mark_rev->color(Clr_GREEN);
}

void PageNewTask::onPaint(GContext& ctx, GCRect const& rc)
{
    ctx.fillRect(rc, m_background);
}

void PageNewTask::onShow(bool show)
{
    focusFirstChild();
    updateLabelsVisible();
}

bool PageNewTask::onSinking(MsgKeyDown const& m, Widget const* target)
{
    /*if (m.key == VK_ESC) {
        // todo: show cancel icon
    }
    else if (m.key == VK_ENTER) {
        if (applyTask()) {
            // todo: show Apply icon
        }
    }*/

    return true;
}

bool PageNewTask::onBubbling(MsgKeyUp const& m, Widget const* target)
{
    return true;
}

bool PageNewTask::onSinking(MsgKeyUp const& m, Widget const* target)
{
    if (m.key == VK_ESC) {
        switchTo(0);
    }
    else if (m.key == VK_ENTER) {
        if (applyTask()) {
            switchTo(0);
        }
    }
    else if (m.key == VK_UP) {
        focusPrev();
        updateLabelsVisible();
    }
    else if (m.key == VK_DOWN) {
        focusNext();
        updateLabelsVisible();
    }

    return true;
}

bool PageNewTask::applyTask()
{
    auto const f = currFocus();
    
    const uint32_t rpm = (uint32_t)m_input_rpm->rpm();
    constexpr bool k_dir = true;

    if (f == m_input_time) {
        const auto v = m_input_time->totalSeconds();
        if (v > 0) {
            m_task.taskTime(Task::Time{rpm, v});
            return app().stepper().setTaskForTime(rpm, k_dir, v);
        }
    }
    if (f == m_input_rev) {
        const auto v = m_input_rev->value();
        if (v > 0) {
            m_task.taskCount(Task::Count{rpm, v});
            return app().stepper().setTaskForRevolutions(rpm, k_dir, v);
        }
    }

    return false;
}

void PageNewTask::updateLabelsVisible()
{
    m_label_rpm->setVisible(m_input_rpm == currFocus());
    m_label_time->setVisible(m_input_time == currFocus());
    m_label_rev->setVisible(m_input_rev == currFocus());

    m_label_esc_rpm->setVisible(m_input_rpm == currFocus());
    m_label_esc_time->setVisible(m_input_time == currFocus());
    m_label_esc_rev->setVisible(m_input_rev == currFocus());

    m_label_mark_time->setVisible(m_input_time == currFocus());
    m_label_mark_rev->setVisible(m_input_rev == currFocus());
}