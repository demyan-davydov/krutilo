// -*- coding:utf-8-unix -*-
/*!
    @file       widget_wizard_page.cpp
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


#include "widget_wizard_page.h"
#include "widget_wizard.h"

WidgetWizardPage::WidgetWizardPage(Widget* parent, Widget::State const& state) 
: Widget(parent, parent->rectLocal(), state)
{

}

WidgetWizard& WidgetWizardPage::parentWizard()
{
    return *(WidgetWizard*)parent();
}

WidgetWizard const& WidgetWizardPage::parentWizard() const
{
    return *(WidgetWizard*)parent();
}

void WidgetWizardPage::setVisibleProperty(bool v)
{
    m_state.visible(v);
}

void WidgetWizardPage::focusFirstChild()
{
    auto t = nextTab();
    if (t) {
        t->setFocus(true);
    }
}

bool WidgetWizardPage::switchTo(int page)
{
    return parentWizard().switchTo(page);
}

bool WidgetWizardPage::switchToNext()
{
    return parentWizard().switchToNext();
}

bool WidgetWizardPage::switchToPrev()
{
    return parentWizard().switchToPrev();
}
