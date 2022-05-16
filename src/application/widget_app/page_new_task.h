// -*- coding:utf-8-unix -*-
/*!
    @file       page_new_task.h
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

#pragma once
#ifndef file__PAGE_NEW_TASK_H
#define file__PAGE_NEW_TASK_H

#include "../widget/widget_wizard_page.h"
#include "../widget/widget_label.h"
#include "../task.h"
#include "brush/brush_solid.h"


class Select;
class InputTime;
class InputNumber;
class SmoothLabel;

class PageNewTask : public WidgetWizardPage
{
    typedef PageNewTask self_t;
    typedef WidgetWizardPage base_t;

public:
    PageNewTask(Widget* parent, Task& task, Widget::State const& initState = Widget::State{});


    void onInit(GContext const& ctx) override;
    
    void onPaint(GContext& ctx, GCRect const& rc) override;

    void onShow(bool show) override;

    bool onSinking(MsgKeyDown const&, Widget const* target) override;

    bool onBubbling(MsgKeyUp const&, Widget const* target) override;

    bool onSinking(MsgKeyUp const&, Widget const* target) override;

    GCBrush const& background() const override { return m_background; }

private:
    bool applyTask();

    void updateLabelsVisible();

private:
    Select* m_input_rpm = nullptr;
    InputTime* m_input_time = nullptr; 
    InputNumber* m_input_rev = nullptr; 
    SmoothLabel* m_label_rpm = nullptr;
    SmoothLabel* m_label_time = nullptr;
    SmoothLabel* m_label_rev = nullptr;

    SmoothLabel* m_label_esc_rpm = nullptr;
    SmoothLabel* m_label_esc_time = nullptr;
    SmoothLabel* m_label_esc_rev = nullptr;

    SmoothLabel* m_label_mark_time = nullptr;
    SmoothLabel* m_label_mark_rev = nullptr;


    Task& m_task;
    GCBrushSolid m_background;
};


#endif // file__PAGE_NEW_TASK_H

