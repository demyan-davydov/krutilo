// -*- coding:utf-8-unix -*-
/*!
    @file       message_queue.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Queue of messages (Msg)
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__MESSAGE_QUEUE_H
#define file__MESSAGE_QUEUE_H

#include "message.h"
#include <queue>
#include <deque>
#include <algorithm>
#include <memory>
#include <cstdint>


class MessageQueue
{
    class MsgWrapper
    {
    public:
        std::unique_ptr<Msg> msg;
        Widget* src;    ///< emitter
        Widget* dst;    ///< destination
    };

    std::deque<MsgWrapper> m_queue;

    void append(Widget* emitter, Widget* destination, std::unique_ptr<Msg>&& e) 
    {
        m_queue.push_back(std::move(MsgWrapper{std::move(e), emitter, destination}));
    }

public:
    template <typename TEvent>
    void append(Widget* emitter, Widget* destination, TEvent const& e) 
    {
        m_queue.push_back(std::move(MsgWrapper{std::move(std::make_unique<TEvent>(e)), emitter, destination}));
    }

    bool empty() const { return m_queue.empty(); }

    size_t size() const { return m_queue.size(); }

    MsgWrapper const& front() const { return m_queue.front(); }

    void pop() { m_queue.pop_front(); }

    void removeBySender(Widget* sender) {
        auto i = std::remove_if(m_queue.begin(), m_queue.end(), [sender](MsgWrapper const& m){ return m.src == sender; } );
        m_queue.erase(i, m_queue.end());
    }

    void removeByDestination(Widget* dst) {
        auto i = std::remove_if(m_queue.begin(), m_queue.end(), [dst](MsgWrapper const& m){ return m.dst == dst; } );
        m_queue.erase(i, m_queue.end());
    }
};


#endif // file__MESSAGE_QUEUE_H

