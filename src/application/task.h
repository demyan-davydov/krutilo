// -*- coding:utf-8-unix -*-
/*!
    @file       task.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-17

    @brief      Working task description
    @description
        Describe timed task or task based on number of revolutions

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__TASK_H
#define file__TASK_H

#include <cstdint>

class Task
{
public:
    enum class Kind 
    {
        None,   // none task
        Time,   // task for time
        Count,  // task for number of revolutions
    };

    struct Time  { uint32_t rpm = {}; uint32_t seconds = {};     bool valid() const { return rpm > 0 && seconds > 0; } };
    struct Count { uint32_t rpm = {}; uint32_t revolutions = {}; bool valid() const { return rpm > 0 && revolutions > 0; } };


    bool empty() const;

    bool ready() const { return !empty(); }

    Kind kind() const { return m_kind; }

    Time const& taskTime() const { return m_time; }

    Count const& taskCount() const { return m_count; }

    int rpm() const 
    {
        switch(m_kind)
        {
            default: return -1;
            case Kind::Time: return m_time.rpm;
            case Kind::Count: return m_count.rpm;
        }
    }

    /// set 'None' task
    void taskNone();

    /// set 'timed' task
    void taskTime(Time const& t);

    /// set 'count' task
    void taskCount(Count const& t);

private:
    Kind m_kind = Kind::None;
    Time m_time;
    Count m_count;
};

#endif // file__TASK_H
