// -*- coding:utf-8-unix -*-
/*!
    @file       main_wizard.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-04

    @brief      Main App Wizard widget
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__WIZARD_MAIN_H
#define file__WIZARD_MAIN_H

#include "../widget/widget_wizard.h"
#include "../task.h"
#include "gc_rect.h"
#include "page_main.h"
#include "page_new_task.h"

class MainWizard : public WidgetWizard
{
    typedef WidgetWizard base_t;
    typedef MainWizard self_t;

public:
    enum class Pages
    {
        Main,
        SetNewTask,
    };

    MainWizard(Widget* parent, GCRect const& rc) : base_t(parent, rc) 
    {
        
    }

    void onInit(GContext const& ctx) override
    {
        appendPage<PageMain>(m_task);
        appendPage<PageNewTask>(m_task);
    }

private:
    Task m_task;
};


#endif // file__WIZARD_MAIN_H

