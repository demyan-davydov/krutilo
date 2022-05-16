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


#include "page_main.h"
#include "button.h"
#include "../widget/widget_progress.h"
#include "../widget/widget_label.h"
#include "colorable_label.h"

#include "labels_win1251.h"
#include "../application.h"
#include "../application_keys.h"
#include "stepper_pwm.h"


PageMain::PageMain(Widget* parent, Task& task, Widget::State const& initState)
: base_t(parent, initState)
, m_task(task)
, m_background(make_565_color(0,50,50))
, m_cooler(NC)
{
    m_stepper_state = app().stepper().state();
    subscribeForIdle();
}

void PageMain::onInit(GContext const& ctx)
{
    const GCPoint step = GCPoint{0, 44};
    GCPoint pt = {160,7};

    m_btn_new_task = &append<Button>(pt, label_new_task, 0, Anchor::TopCenter); pt += step;

    //auto btn_start_w = std::max(Button::calcWidthFor(label_start), 
    //                        std::max(Button::calcWidthFor(label_pause), 
    //                            Button::calcWidthFor(label_continue)));
    auto btn_start_w = m_btn_new_task->size().sx;
    m_btn_start = &append<Button>(pt, label_start, btn_start_w, Anchor::TopCenter); pt += step;
    m_btn_start->contentAnchor(Anchor::MidCenter);

    // progress bar
    {
        constexpr int k_progress_heigth = 25;
        GCRect rcProgress = rectLocal();
        rcProgress.ry.resizeFromB(k_progress_heigth);
        m_progress = &append<WidgetProgressBar>(rcProgress);
    }

    // labels
    {
        constexpr GCCoord progress_gap = 5;
        constexpr GCCoord progress_gap2 = 5;

        GCPoint pt2 = m_progress->rect().topRight() - GCPoint_Y(progress_gap);
        m_label_progress = &append<ColorableLabel>(pt2, Anchor::BtmRight, "100%");
        m_label_progress->contentAnchor(Anchor::MidRight);
        m_label_progress->color(Clr_LIGHT_YELLOW);

        pt2 = m_progress->rect().topLeft() + GCPoint(6, -(progress_gap+progress_gap2));
        auto& lbl_remains_name = append<ColorableLabel>(pt2, Anchor::BtmLeft, label_remains, uistyle().smallFont());
        m_label_remains = &append<ColorableLabel>(lbl_remains_name.rect().bottomRight()+GCPoint_Y(progress_gap2), 
                                                  Anchor::BtmLeft, 10);
        lbl_remains_name.color(Clr_LIGHT_GRAY);
        m_label_remains->color(Clr_LIGHT_GRAY);

        // RPM
        {
            pt2 = GCPoint(210,106);
            m_label_rpm = &append<ColorableLabel>(pt2, Anchor::TopCenter, "---", uistyle().bigNumericFont());
            m_label_rpm->contentAnchor(Anchor::MidCenter);
            m_label_rpm->color(Clr_LEMON_CHIFFON);
            m_label_rpm->setVisible(false);
            m_label_rpm_subscript = &append<ColorableLabel>(m_label_rpm->rect().bottomCenter(), 
                                                            Anchor::TopCenter, label_rpm_short, uistyle().smallFont());
            m_label_rpm_subscript->color(Clr_GRAY);
            m_label_rpm_subscript->setVisible(false);
        }

        // task description labels
        {
            pt2 = m_label_rpm_subscript->rect().bottomLeft() - GCPoint_X(15);

            m_label_task = &append<ColorableLabel>(pt2, Anchor::BtmRight, 9, uistyle().smallFont());
            m_label_task->color(Clr_GRAY);
            m_label_task->contentAnchor(Anchor::MidCenter);
            m_label_task->setVisible(false);

            pt2 = m_label_task->rect().topCenter();
            m_label_task_desc = &append<ColorableLabel>(pt2, Anchor::BtmCenter, 8, uistyle().normalFont());
            m_label_task_desc->color(Clr_WHITE);
            m_label_task_desc->contentAnchor(Anchor::MidCenter);
            m_label_task_desc->setVisible(false);


        }

        // no task
        {
            pt2 = GCPoint(160,120);
            m_label_no_task = &append<ColorableLabel>(pt2, Anchor::TopCenter, label_no_task, uistyle().normalFont());
            m_label_no_task->color(Clr_GRAY);
        }

        //const char* lll = "\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90--";
        //append<ColorableLabel>(m_label_remains->rect().topLeft(), Anchor::BtmLeft, lll, uistyle().normalFont());
    }
}

void PageMain::onPaint(GContext& ctx, GCRect const& rc)
{
    ctx.fillRect(rc, m_background);
}

void PageMain::onShow(bool show)
{
    focusFirstChild();

    auto& stepper = app().stepper();

    m_btn_start->setEnabled(stepper.canResume());
    m_btn_new_task->setEnabled(!stepper.isRotated());

    // setup rpm label
    if (m_task.empty())
    {
        m_label_rpm->setVisible(false);
        m_label_rpm_subscript->setVisible(false);
        m_label_task_desc->setVisible(false);
        m_label_task->setVisible(false);
        m_label_no_task->setVisible(true);
    }
    else
    {
        //auto rpm = stepper.rpm();
        auto rpm = m_task.rpm();
        char buff[16];

        if (rpm > 0) {
            sprintf(buff, "%d", (int)rpm);
            m_label_rpm->text(buff);
        }

        if (m_task.kind() == Task::Kind::Count) {
            sprintf(buff, "%d", (int)m_task.taskCount().revolutions);
            m_label_task_desc->text(buff);
            m_label_task->text(label_task_subscript_rpm);
        }
        else { // 
            int m = m_task.taskTime().seconds / 60;
            int mm = m % 60; m -= mm;
            int hh = m / 60;

            sprintf(buff, label_pattern_time_task, hh, mm);
            m_label_task_desc->text(buff);
            m_label_task->text(label_task_subscript_time);
        }
        //else {
        //    m_label_rpm->text("---");
        //}

        m_label_no_task->setVisible(false, false);
        m_label_rpm->setVisible(true);
        m_label_rpm_subscript->setVisible(true);
        m_label_task_desc->setVisible(true);
        m_label_task->setVisible(true);
    }

    // update progress value
    m_progress->value(app().stepper().progress());

    updateLabels();
}

void PageMain::onIdle()
{
    // check steper state
    {
        auto s = app().stepper().state();
        if (s != m_stepper_state) {
            m_stepper_state = s;
            onStepperStateChanged();
        }
    }

    // update progressbar if needed
    {
        m_progress->value(app().stepper().progress());
    }

    updateLabels();
}

bool PageMain::onBubbling(MsgKeyUp const& m, Widget const* target)
{
    return true;
}

bool PageMain::onSinking(MsgKeyUp const& m, Widget const* target)
{
    if (m.key == VK_UP) {
        focusPrev();
    }
    else if (m.key == VK_DOWN) {
        focusNext();
    }
    else if (m.key == VK_ENTER) {
        if (currFocus() == m_btn_new_task) {
            switchTo(1);
        }
        if (currFocus() == m_btn_start) {
            onStartTask();
        }
    }
    
    return true;
}

void PageMain::setupButtons(bool enableNewTask, bool enableStart, const char* startText, WidgetProgressBar::State progressState)
{
    m_btn_new_task->setEnabled(enableNewTask);
    m_btn_start->setEnabled(enableStart);
    m_btn_start->text(startText);
    m_progress->state(progressState);
}

void PageMain::onStepperStateChanged()
{
    using PState = WidgetProgressBar::State;

    switch(m_stepper_state)
    {
        case StepperPwm::State::NeedSetup:     setupButtons(true, false, label_start, PState::Progressed); break;
        case StepperPwm::State::Ready:         setupButtons(true, true,  label_start, PState::Progressed); break;
        case StepperPwm::State::Paused:        setupButtons(true, true,  label_continue, PState::Paused); break;
        case StepperPwm::State::Finished:      setupButtons(true, true,  label_start, PState::Finished); break;
        case StepperPwm::State::Runned:        setupButtons(false, true, label_pause, PState::Progressed); break;
    }

    // setup cooler
    if (m_cooler.is_connected())
    {
        const bool cooler_enabled = m_stepper_state == StepperPwm::State::Runned;
        m_cooler.write(cooler_enabled ? 1 : 0);
    }
}

void PageMain::updateLabels()
{
    auto& stepper = app().stepper();
    auto remains = stepper.remainedSeconds();
    const int rhh = remains / (60 * 60); remains -= rhh * 60 * 60;
    const int rmm = remains / (60); remains -= rmm * 60;
    const int rss = remains;

    char buff[32];
    sprintf(buff, label_pattern_remains, rhh, rmm, rss);
    m_label_remains->text(buff);

    sprintf(buff, "%d%%", (int)(stepper.progress()*100) );
    m_label_progress->text(buff);
}

void PageMain::onStartTask()
{
    auto& stepper = app().stepper();

    if (stepper.isFullyStop()) {
        if (stepper.isFinished()) {
            stepper.rewind();
        }
        stepper.rotate();
    }
    else {
        stepper.pause();
    }
}
