// -*- coding:utf-8-unix -*-
/*!
    @file       widget_wizard.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-04

    @brief      Wizard widget
    @description
        Show text content

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__WIDGET_WIZARD_H
#define file__WIDGET_WIZARD_H

#include "widget.h"
#include "widget_wizard_page.h"
#include <type_traits>


class WidgetWizard : public Widget
{
    typedef WidgetWizard self_t;
    typedef Widget base_t;
    WidgetWizardPage* m_curr = nullptr;

    void switchToPage(WidgetWizardPage* page)
    {
        if (m_curr != page)
        {
            if (m_curr) {
                m_curr->setVisible(false, false);
            }
            if (page) {
                page->setVisible(true, false);
            }
            m_curr = page;
        }  
    }

    // todo: disable
    template<typename TWidget, typename... Args>
    TWidget& append(Args&&... args) = delete;

public:
    WidgetWizard(Widget* parent, GCRect const& rc) : base_t(parent, rc) 
    {
    }

    template<typename TWidget, typename... Args>
    TWidget& appendPage(Args&&... args)
    {
        static_assert(std::is_base_of<WidgetWizardPage, TWidget>::value, 
            "You may only add objects derived from WidgetWizardPage");

        TWidget& el = base_t::append<TWidget>(std::forward<Args>(args)...);
        
        // show first added page
        if (!m_curr) {
            el.m_state.visible(true);
            m_curr = &el;
        }
        else {
            el.m_state.visible(false);
        }
        
        return el;
    }

    WidgetWizardPage* currPage()
    {
        return m_curr;
    }

    bool switchToPrev()
    {
        if (m_curr)
        {
            auto sibling = m_curr->prevSibling();
            if (sibling) {
                switchToPage((WidgetWizardPage*)sibling);
                return true;
            }
        }
        return false;
    }

    bool switchToNext()
    {
        if (m_curr)
        {
            auto sibling = m_curr->nextSibling();
            if (sibling) {
                switchToPage((WidgetWizardPage*)sibling);
                return true;
            }
        }
        return false;
    }

    bool switchTo(uint32_t idx)
    {
        if (idx < childCount())
        {
            Widget* next = nthChild(idx);
            if (next) {
                switchToPage((WidgetWizardPage*)next);
                return true;
            }
        }
        return false;
    }

    /// return selected page index or -1 if no page in wizard
    int currPageIndex() const { return m_curr->parentOrder(); }
};


#endif // file__WIDGET_WIZARD_H

