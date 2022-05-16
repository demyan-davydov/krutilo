// -*- coding:utf-8-unix -*-
/*!
    @file       stepper.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-17

    @brief      Wrapper for our stepper
    @description


    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/


#pragma once
#ifndef file__STEPPER_H
#define file__STEPPER_H

/*
#include "stepper_pwm.h"


class Stepper
{
    typedef StepperPwm base_t;

    static constexpr uint32_t k_stepsPerRev = 200 * 16;

public:
    typedef StepperPwm::State State; 

    /// @brief Create a new AsyncStepper.
    /// @param stepPin    A4988 Step pin
    /// @param enablePin  A4988 Enable pin
    /// @param dirPin     A4988 Direction pin
    Stepper(PinName stepPin, PinName dirPin, PinName enablePin) 
    : m_stepper(stepPin, dirPin, enablePin)
    {
    }

    bool clearTask() { return m_stepper.clearTask(); }

    /// @brief set task for unlimited rotation
    /// @return success 
    bool setTaskUnlimited(uint32_t pulsesPerSecond, bool dir);

    /// @brief set task for steps-count-limited rotation. steps can not be equal 0
    /// @return success
    bool setTaskForRevolutions(uint32_t rpm, bool dir, uint32_t rev)
    {
        const uint64_t steps = (uint64_t)k_stepsPerRev*rev;
        return m_stepper.setTaskForSteps(base_t::ppsFromRpm(rpm, k_stepsPerRev), dir, steps);
    }

    /// @brief set task for time-limited rotation. time can not be equal 0
    /// @return success
    bool setTaskForTime(uint32_t rpm, bool dir, uint32_t seconds)
    {
        return m_stepper.setTaskForTime(base_t::ppsFromRpm(rpm, k_stepsPerRev), dir, seconds);
    }

    void rotate() { m_stepper.rotate(); }

    void pause() { m_stepper.pause(); }

    /// @brief has task for execution
    bool hasTask() const { return m_stepper.hasTask(); }

    /// @brief is task finished?
    bool done() const { return m_stepper.done(); }

    /// @brief rewind limited task
    /// @return true if current task can be rewinded (i.e. is limited task)
    bool rewind() { return m_stepper.rewind(); }

    /// @brief Set 'enable' state for stopped motor. Running motor always enabled
    void hardStopFlag(bool e) { m_stepper.hardStopFlag(e); }

    /// @brief Is motor fully stops?
    bool isFullyStop() const { return m_stepper.isFullyStop(); }

    /// @brief Is motor stops because task is finished?
    bool isFinised() const { return m_stepper.isFinished(); }

    /// @brief Is now rotated?
    bool isRotated() const { return m_stepper.isRotated(); }

    /// @brief last rotate direction
    bool dir() const { return m_stepper.dir(); }

    /// @brief Get 'Enabled' policy for motor.
    /// Is applied when motor stops
    bool hardStopFlag() const { return m_stepper.hardStopFlag(); }

    // progress information

    /// @brief Stepper state info    
    State state() const { return m_stepper.state(); }

    /// @brief Stepper has task, if it stop now, resume() can be called
    bool canResume() const { return m_stepper.canResume(); }

    /// @brief Return total count of steps to perform current task
    uint64_t stepsLimit() const { return m_stepper.stepsLimit(); }

    /// @brief Return currently performed steps from beginning of task
    uint64_t stepsCount() const { return m_stepper.stepsCount(); }

    /// @brief Return number of seconds for current task
    uint32_t totalSeconds() const { return m_stepper.totalSeconds(); }

    /// @brief Return seconds spended since current task beginned
    uint32_t elapsedSeconds() const { return m_stepper.elapsedSeconds(); }

    /// @brief Return seconds remains to finish the current task
    uint32_t remainedSeconds() const { return m_stepper.remainedSeconds(); }

    /// @brief Return RPM or -1 if task is not set
    int rpm() const { return m_stepper.rpm(k_stepsPerRev); }

    /// @brief Progress [0..1] of current task.
    /// Then task is finished, progress is 1
    float progress() const { return m_stepper.progress(); }

private: 
    StepperPwm m_stepper;
};
*/

#endif // file__STEPPER_H
