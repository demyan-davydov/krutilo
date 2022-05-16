// -*- coding:utf-8-unix -*-
/*!
    @file       widget_wizard_page.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-06

    @brief      page for Wizard widget
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__WIDGET_WIZARD_PAGE_H
#define file__WIDGET_WIZARD_PAGE_H

#include "widget.h"

class WidgetWizard;

class WidgetWizardPage : public Widget
{
    friend class WidgetWizard;

    WidgetWizard& parentWizard();
    WidgetWizard const& parentWizard() const;

protected:
    // can be created only by WidgetWizard or derived classes
    WidgetWizardPage(Widget* parent, Widget::State const& state);

    /// just set property, not repaint
    /// used by WidgetWizard then this page will be added
    void setVisibleProperty(bool v);

    /// Set focus for first focusable child
    void focusFirstChild();

public:
    bool switchTo(int page);
    bool switchToNext();
    bool switchToPrev();
};


#endif // file__WIDGET_WIZARD_PAGE_H
