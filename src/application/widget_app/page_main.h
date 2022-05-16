// -*- coding:utf-8-unix -*-
/*!
    @file       page_main.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      main (first) wizard page
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__PAGE_MAIN_H
#define file__PAGE_MAIN_H

#include "../widget/widget_wizard_page.h"
#include "../widget/widget_progress.h"
#include "../task.h"
#include "stepper_pwm.h"

#include "brush/brush_solid.h"
#include "mbed.h"


class Button;
class WidgetLabel;
class ColorableLabel;

class PageMain : public WidgetWizardPage
{
    typedef PageMain self_t;
    typedef WidgetWizardPage base_t;

public:
    PageMain(Widget* parent, Task& task, Widget::State const& initState = Widget::State{});

    void onInit(GContext const& ctx) override;
    
    void onPaint(GContext& ctx, GCRect const& rc) override;

    void onShow(bool show) override;

    void onIdle() override;

    bool onBubbling(MsgKeyUp const&, Widget const* target) override;

    bool onSinking(MsgKeyUp const&, Widget const* target) override;

    GCBrush const& background() const override { return m_background; }

private:

    void setupButtons(bool enableNewTask, bool enableStart, const char* startText, WidgetProgressBar::State progressState);

    void onStepperStateChanged();

    void updateLabels();

    void onStartTask();

private:
    Button* m_btn_new_task = nullptr;
    Button* m_btn_start = nullptr;
    WidgetProgressBar* m_progress = nullptr;
    ColorableLabel* m_label_progress = nullptr;
    ColorableLabel* m_label_remains = nullptr;
    ColorableLabel* m_label_rpm = nullptr;
    ColorableLabel* m_label_rpm_subscript = nullptr;
    ColorableLabel* m_label_no_task = nullptr;
    ColorableLabel* m_label_task = nullptr;
    ColorableLabel* m_label_task_desc = nullptr;

    Task& m_task;
    GCBrushSolid m_background;
    StepperPwm::State m_stepper_state;

    DigitalOut m_cooler;
};


#endif // file__PAGE_MAIN_H

