// -*- coding:utf-8-unix -*-
/*!
    @file       message.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Application messages like Quit, Paint, KeyUp, etc.
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__MESSAGE_2_H
#define file__MESSAGE_2_H

#include "ticker_api.h" // timestamp;
#include <memory>

class Widget;

enum MessageKind
{
    MSG_Quit,       // 
    MSG_Init,       // init window after append
    MSG_KeyDown,    // kew ws pressed
    MSG_KeyUp,      // key was released
    MSG_Paint,      // need repaint window
    MSG_Move,       // widget is moved

    MSG_User,       // last predefined message, use MSG_User + N to define your own messages
};

struct Msg
{
    //Msg(Msg const&) = delete;

    virtual MessageKind kind() const = 0;
    Msg() {}
    virtual ~Msg() {}

    // if event 'e' can be combined witch this event, just combine it, do not a new event to loop
    //virtual bool combineWitch(Evt const& e) { return false; }
};

template<MessageKind k_kind>
struct Msg_T : public Msg
{
    Msg_T() {}
    virtual MessageKind kind() const override { return k_kind; };
};



/// structs witch Event description

struct MsgInit : public Msg_T<MSG_Init>
{
};

struct MsgMove : public Msg_T<MSG_Move>
{
    MsgMove(GCRect const& r) : rc(r) {}
    GCRect rc;
};

struct MsgKeyUp : public Msg_T<MSG_KeyUp>
{
    MsgKeyUp(char c) : key(c) {}
    char key;
};

struct MsgKeyDown : public Msg_T<MSG_KeyDown>
{
    MsgKeyDown(char c) : key(c) {}
    char key;
};

struct MsgPaint : public Msg_T<MSG_Paint>
{
    MsgPaint(GCRect const& r) : rc(r) {}
    MsgPaint() : rc{-1,-1} {}
    GCRect rc;              ///< area to repaint in local coordiantes
};

#endif // file__MESSAGE_H

