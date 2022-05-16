// -*- coding: windows-1251 -*-
/*!
    Это русский текст чтобы работало автоопределение кодировки.
    Съешь ещё этих мягких французских булок, да выпей чаю.
    Иван Родил Девчонку, Велел Тащить Пелёнку.

    @file       labels_win1251.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Содержит отображаемые надписи интерфейса
    @description
        Т.к. шрифты из gfxfonts.h содержат определения для символов 
        в кодировке windows-1251, разумнее всего хранить их в файле 
        с такой-же кодировкой.

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__LABELS_WIN1251_H
#define file__LABELS_WIN1251_H


constexpr const char* label_new_task = "Новая задача";

constexpr const char* label_no_task = "Создайте задачу";
constexpr const char* label_task = "Задача";
constexpr const char* label_task_subscript_rpm = "обороты";
constexpr const char* label_task_subscript_time = "чч:мм";
constexpr const char* label_pattern_time_task = "%02d:%02d";

constexpr const char* label_start = "Начать";
constexpr const char* label_pause = "Пауза";
constexpr const char* label_continue = "Продолжить";

constexpr const char* label_speed = "Скорость";
constexpr const char* label_time = "Время";
constexpr const char* label_revolutions = "Обороты";

constexpr const char* label_cancel = "\x89"; // "\x86"
constexpr const char* label_check = "\x8b";

constexpr const char* label_rpm_short = "об/мин";
constexpr const char* label_hours_short = "ч";
constexpr const char* label_minutes_short = "м";

constexpr const char* label_remains = "Осталось";
constexpr const char* label_pattern_remains = " %2d:%02d:%02d";


#endif // file__LABELS_WIN1251_H

