// -*- coding: windows-1251 -*-
/*!
    ��� ������� ����� ����� �������� ��������������� ���������.
    ����� ��� ���� ������ ����������� �����, �� ����� ���.
    ���� ����� ��������, ����� ������ ������.

    @file       labels_win1251.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      �������� ������������ ������� ����������
    @description
        �.�. ������ �� gfxfonts.h �������� ����������� ��� �������� 
        � ��������� windows-1251, �������� ����� ������� �� � ����� 
        � �����-�� ����������.

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__LABELS_WIN1251_H
#define file__LABELS_WIN1251_H


constexpr const char* label_new_task = "����� ������";

constexpr const char* label_no_task = "�������� ������";
constexpr const char* label_task = "������";
constexpr const char* label_task_subscript_rpm = "�������";
constexpr const char* label_task_subscript_time = "��:��";
constexpr const char* label_pattern_time_task = "%02d:%02d";

constexpr const char* label_start = "������";
constexpr const char* label_pause = "�����";
constexpr const char* label_continue = "����������";

constexpr const char* label_speed = "��������";
constexpr const char* label_time = "�����";
constexpr const char* label_revolutions = "�������";

constexpr const char* label_cancel = "\x89"; // "\x86"
constexpr const char* label_check = "\x8b";

constexpr const char* label_rpm_short = "��/���";
constexpr const char* label_hours_short = "�";
constexpr const char* label_minutes_short = "�";

constexpr const char* label_remains = "��������";
constexpr const char* label_pattern_remains = " %2d:%02d:%02d";


#endif // file__LABELS_WIN1251_H

