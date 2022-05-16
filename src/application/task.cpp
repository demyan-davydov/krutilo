// -*- coding:utf-8-unix -*-
/*!
    @file       task.cpp
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-17

    @brief      Task description
    @description
        Describe timed task or task based on number of revolutions

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#include "task.h"


bool Task::empty() const
{
    return  (m_kind == Kind::None) || 
            (m_kind == Kind::Time && !m_time.valid()) ||
            (m_kind == Kind::Count && !m_count.valid());
}

void Task::taskNone()
{
    m_kind = Kind::None;
}

void Task::taskTime(Time const& t)
{
    m_kind = Kind::Time;
    m_time = t;
}

void Task::taskCount(Count const& t)
{
    m_kind = Kind::Count;
    m_count = t;
}

