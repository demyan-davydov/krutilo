// -*- coding:utf-8-unix -*-
/*!
    @file       stepper_pwm.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Interfacing with stepper through step/dir driver, such as A4988, DRV8825
    @description
        This class control stepper through PwmOut & modified InterruptIn classes.
        This will allow you to run the stepper to perform a certain number of revolution 
        and sto[ automatically after. It implements "run and forget" strategy.
        
        It keeps the MCU free for any other operations. It works much and much
        faster then implementaions bulded on Ticker class.

        It has two kinds of run methods:
        * rotateForSteps(), rotateForTime() -- launches a task, uses 
          modified InterruptIn to check complition of operation.
        * rotate() -- just run stepper, using only PwmOut to generate pulses.
        
        rotateForSteps(), rotateForTime() takes a little(really little) bit more 
        resources then rotate(). 
        
        KNOWN ISSUES:
        On STM32F103 i observed strange problems after rotateForSteps() was runned: 
        In parrallel with rotateForSteps() i ran the following code:
            somePin.mode(PinMode::PullDown);
            auto state = somePin.read();
            somePin.mode(PinMode::PullUp);
        And this code worked unstable, i observe something like hight frequency 
        interference. -- somePin.read() returned random values. The problem 
        disappeared when i decreased rotateForSteps() frequency to about 5 pulses 
        per second. It also disappears then i remove the temporary mode switching
        (PullDown/PullUp) from my code.

        Note: As i realise, PwmOut may not be connected to any pins, for STM MCU's 
        it must be digital pins.

        TESTED ON: 
            STM32F103 (with A4988)

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__ASYNC_STEPPER_H
#define file__ASYNC_STEPPER_H
 
#include "mbed.h"
#include <cstdint>
//#include "pwmout_x.h"


class StepperPwm
{
    static constexpr int k_hardStop_Enable = 1;
    static constexpr int k_hardStop_Disable = k_hardStop_Enable ? 0 : 1;

public:
    enum class State : uint8_t
    {
        NeedSetup,  ///< not yet has task (stopped)
        Ready,      ///< has task but not yet runned (stopped)
        Paused,     ///< runned but paused (stopped)
        Runned,     ///< rotated
        Finished,   ///< task finished (stopped)
    };

    /// @brief Create a new AsyncStepper.
    /// @param stepPin    A4988 Step pin
    /// @param enablePin  A4988 Enable pin
    /// @param dirPin     A4988 Direction pin
    StepperPwm(PinName stepPin, PinName dirPin, PinName enablePin, int stepsPerRevolution);
 
    /// @brief Destrutor of AsyncStepper
    ~StepperPwm();

    /// @brief remive any task and stop motor if it rotated
    bool clearTask();

    /// @brief set task for unlimited rotation
    /// @return success 
    bool setTaskUnlimited(uint32_t rpm, bool dir);

    /// @brief set task for steps-count-limited rotation. steps can not be equal 0
    /// @return success
    bool setTaskForSteps(uint32_t rpm, bool dir, uint64_t steps);

    /// @brief set task for n - full revolutions. revolutions can not be equal 0
    /// @return success
    bool setTaskForRevolutions(uint32_t rpm, bool dir, uint32_t revolutions);

    /// @brief set task for time-limited rotation. time can not be equal 0
    /// @return success
    bool setTaskForTime(uint32_t rpm, bool dir, uint32_t seconds);

    /// @brief Start motor rotation
    void rotate();

    /// @brief Pause motor rotation
    void pause();

    /// @brief has task for execution
    bool hasTask() const;

    /// @brief is task finished?
    bool done() const; 

    /// @brief rewind limited task
    /// @return true if current task can be rewinded (i.e. is limited task)
    bool rewind();


    /// @brief Set 'enable' state for stopped motor. Running motor always enabled
    void hardStopFlag(bool e);

    /// @brief Convert RPM (revolutions per second) to PPS(pulses per second)
    static uint32_t ppsFromRpm(uint32_t rpm, uint32_t stepsPerRevolution);

    /// @brief Is motor fully stops?
    bool isFullyStop() const;

    /// @brief Is motor stops because task is finished?
    bool isFinished() const;

    /// @brief Is now rotated?
    bool isRotated() const;

    /// @brief Stepper has task, if it stop now, resume() can be called
    bool canResume() const;

    /// @brief last rotate direction
    bool dir() const { return m_dir; }

    /// @brief Get 'Enabled' policy for motor.
    /// Is applied when motor stops
    bool hardStopFlag() const;

    // progress information

    /// @brief Stepper state info    
    State state() const;


    /// @brief Return total count of steps to perform current task
    uint64_t stepsLimit() const;

    /// @brief Return currently performed steps from beginning of task
    uint64_t stepsCount() const;

    /// @brief Return number of seconds for current task
    uint32_t totalSeconds() const;

    /// @brief Return seconds spended since current task beginned
    uint32_t elapsedSeconds() const;

    /// @brief Return seconds remains to finish the current task
    uint32_t remainedSeconds() const;

    /// @bruef Return RPM or -1 if task is not set
    int rpm() const;

    /// @brief Progress [0..1] of current task.
    /// Then task is finished, progress is 1
    float progress() const;

private:

    /// @brief clear task inforamtion
    //void h_taskClear();

    /// @brief init task information for new task
    //void h_taskInit(uint32_t period_us, bool dir, uint64_t steps = 0);

    /// @brief resume paused rotation
    //void h_resume();

    /// @brief Initiate infinite rotation
    //void h_rotateByPeriod(uint32_t period_us, bool dir, uint64_t steps = 0);

    /// @brief Initiate limited rotation (start task)
    //void h_rotateForStepsByPeriod(uint32_t period_us, bool dir, uint64_t steps);

    /// @brief Supposed stepper is rotated and stop it
    void h_stopAnyway();

    /// @brief calculate period (in us) for specified RPM and pulses-Per-Revolutions
    static uint64_t h_calcPulsePeriodFromRpm(uint32_t rmp, uint32_t stepsPerRev);

    /// @brief calculate period (in us) for specified PPS (pulses per second)
    static uint64_t h_calcPulsePeriod(uint32_t pulsesPerSecond);

    /// @brief Apply the current to the stepper motor
    void h_applyEnable(bool e);

    /// @brief Irq handler called on each pulse of PWM (non static)
    void h_irq_handler();

    /// @brief Irq handler called on each pulse of PWM
    static void h_irq_handler_static(uint32_t id, gpio_irq_event event);

private:
    PwmOut m_pwm; ///< Used to form impulses. Connected to 'step' pin
    DigitalOut m_dirOut; ///< Dir pin
    DigitalOut m_enableOut; ///< Dir pin
    gpio_irq_t m_gpio_irq; ///< IRQ for control number of performed impulses

    bool m_enableState = false; ///< enable state for non rotated stepper
    volatile bool m_rotated = false; ///< is stepper rotated now
    volatile bool m_dir = false; ///< last rotate direction
    volatile uint64_t m_stepsLimit = 0; ///< count of steps to do
    volatile uint64_t m_stepsCount = 0; ///< performed steps
    uint32_t m_period_us = 0; ///< period for PWM ticks
    uint32_t m_spr = 0; ///< steps per revolution 
};
 
 
#endif // file__ASYNC_STEPPER_H
