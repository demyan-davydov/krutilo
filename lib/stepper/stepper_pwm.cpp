// -*- coding:utf-8-unix -*-
/*!
    @file       stepper_pwm.cpp
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
            STM32F103

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#include "stepper_pwm.h"
#include <limits>

StepperPwm::StepperPwm(PinName stepPin, PinName dirPin, PinName enablePin, int stepsPerRevolution) 
: m_pwm(stepPin)
, m_dirOut(dirPin)
, m_enableOut(enablePin)
, m_spr(stepsPerRevolution)
{
    h_applyEnable(m_enableState);

    //dirOut.write( 0 );
    m_pwm.period_ms(1000);
    m_pwm.write(0.f); // no signal;

    core_util_critical_section_enter();
    gpio_irq_init(&m_gpio_irq, stepPin, (&h_irq_handler_static), (uint32_t)this);
    gpio_irq_disable(&m_gpio_irq);
    gpio_irq_set(&m_gpio_irq, IRQ_RISE, 1);
    core_util_critical_section_exit();
}

StepperPwm::~StepperPwm()
{
    gpio_irq_free(&m_gpio_irq);
}


bool StepperPwm::clearTask()
{
    if (isFullyStop()) {
        m_stepsCount = 0;
        m_period_us = 0;
        return true;
    }
    return false;
}

bool StepperPwm::setTaskUnlimited(uint32_t rpm, bool dir)
{
    if (isFullyStop()) {
        m_period_us = h_calcPulsePeriodFromRpm(rpm, m_spr);
        m_stepsLimit = std::numeric_limits<uint64_t>::max();
        m_stepsCount = 0;
        m_dir = dir;
        return true;
    }
    return false;
}

bool StepperPwm::setTaskForSteps(uint32_t rpm, bool dir, uint64_t steps)
{
    if (isFullyStop()) {
        m_period_us = h_calcPulsePeriodFromRpm(rpm, m_spr);
        m_stepsLimit = steps*2;
        m_stepsCount = 0;
        m_dir = dir;
        return true;
    }
    return false;
}

bool StepperPwm::setTaskForRevolutions(uint32_t rpm, bool dir, uint32_t revolutions)
{
    return setTaskForSteps(rpm, dir, (uint64_t)revolutions * m_spr);
}

bool StepperPwm::setTaskForTime(uint32_t rpm, bool dir, uint32_t seconds)
{
    if (isFullyStop()) {
        auto period_us = h_calcPulsePeriodFromRpm(rpm, m_spr);
        auto steps = ((uint64_t)seconds * (uint64_t)1000000 + period_us/2)/(period_us);

        m_period_us = period_us;
        m_stepsLimit = steps;
        m_stepsCount = 0;
        m_dir = dir;
        return true;
    }
    return false;
}

void StepperPwm::rotate()
{
    if (isFullyStop() && hasTask())
    {
        m_dirOut.write(m_dir);
        h_applyEnable(true);

        // is limited rotation, we need IRQ to control least number of impulses
        if (m_stepsLimit != std::numeric_limits<uint64_t>::max()) 
        {
            core_util_critical_section_enter();
            gpio_irq_enable(&m_gpio_irq);
            core_util_critical_section_exit();
        }

        m_pwm.period_us(m_period_us);
        m_pwm.write(0.5);
        m_rotated = true;
    }
}

void StepperPwm::pause()
{
    if (isRotated()) {
        h_stopAnyway();
    }
}

bool StepperPwm::hasTask() const
{
    return m_period_us != 0;
}

bool StepperPwm::done() const
{
    return m_period_us && m_stepsCount >= m_stepsLimit;
}

bool StepperPwm::rewind()
{
    m_stepsCount = 0;
    return hasTask();
}

void StepperPwm::hardStopFlag(bool e)
{
    if (m_enableState != e) {
        m_enableState = e;

        if (isFullyStop())
            h_applyEnable(e);
    }
}

uint32_t StepperPwm::ppsFromRpm(uint32_t rpm, uint32_t stepsPerRevolution) // static
{
    return (rpm * stepsPerRevolution + 30)/ 60;
}

bool StepperPwm::isFullyStop() const
{
    return !m_rotated;
}

bool StepperPwm::isFinished() const
{
    return !m_rotated && (m_stepsCount >= m_stepsLimit);
}

bool StepperPwm::isRotated() const
{
    return m_rotated;
}

bool StepperPwm::canResume() const
{
    return !m_rotated && hasTask();
}

bool StepperPwm::hardStopFlag() const
{
    return m_enableState;
}

StepperPwm::State StepperPwm::state() const
{
    if (hasTask())
    {
        if (m_rotated) {
            return State::Runned;
        }
        else {
            if (m_stepsCount == 0) {
                return State::Ready;
            }
            else if (m_stepsCount >= m_stepsLimit) {
                return State::Finished;
            }
            else {
                return State::Paused;
            }
        }
    }

    return State::NeedSetup;
}

uint64_t StepperPwm::stepsLimit() const
{
    return m_stepsLimit;
}

uint64_t StepperPwm::stepsCount() const
{
    return m_stepsCount;
}

uint32_t StepperPwm::totalSeconds() const
{
    return (m_stepsLimit * m_period_us + 1000000/2) / 1000000; 
}

uint32_t StepperPwm::elapsedSeconds() const
{
    return (m_stepsCount * m_period_us + 1000000/2) / 1000000;
}

uint32_t StepperPwm::remainedSeconds() const
{
    const auto remained = m_stepsLimit - m_stepsCount;
    return (remained * m_period_us + 1000000/2) / 1000000;
}

int StepperPwm::rpm() const
{
    if (m_period_us) {
        return ((uint64_t)(m_spr * 6) + m_period_us/2) / m_period_us;
    }
    return -1;
}

float StepperPwm::progress() const
{
    return (m_stepsLimit > 0) ? (float)m_stepsCount / (float)m_stepsLimit : 0.0f;
}
/*
void StepperPwm::h_rotateByPeriod(uint32_t period_us, bool dir, uint64_t steps)
{
    m_dirOut.write(dir);
    h_applyEnable(true);

    m_period_us = period_us;
    m_stepsLimit = steps*2;
    m_stepsCount = 0;
    m_rotated = true;
    m_dir = dir;

    // setup pwm
    m_pwm.period_us(m_period_us);
    m_pwm.write(0.5);
}

void StepperPwm::h_rotateForStepsByPeriod(uint32_t period_us, bool dir, uint64_t steps)
{
    if (steps)
    {
        // initiate rotation
        h_rotateByPeriod(period_us, dir, steps);

        // setup steps callback
        core_util_critical_section_enter();
        gpio_irq_enable(&m_gpio_irq);
        core_util_critical_section_exit();
    }
}
*/
void StepperPwm::h_stopAnyway()
{
    __disable_irq();

    core_util_critical_section_enter();
    gpio_irq_disable(&m_gpio_irq);
    core_util_critical_section_exit();

    m_pwm.write(0);
    m_rotated = false;
    //m_stepsCount = m_stepsLimit;
    //m_stepsCount = 0;
    //m_stepsLimit = 0;
    //m_period_us = 0;
    __enable_irq();

    h_applyEnable(m_enableState); // restore enable state
}

uint64_t StepperPwm::h_calcPulsePeriodFromRpm(uint32_t rpm, uint32_t stepsPerRev) // static
{
    constexpr uint64_t us_inMinute = (uint64_t)1000000 * 60;
    const uint32_t rrr = stepsPerRev * rpm * 2; // * 2 -- 2 pulses for one step
    return (us_inMinute + rrr/2) / rrr;
}

uint64_t StepperPwm::h_calcPulsePeriod(uint32_t pulsesPerSecond) // static
{
    constexpr uint64_t us_inSecond = (uint64_t)1000000;
    return (us_inSecond + pulsesPerSecond/2) / pulsesPerSecond;
}

void StepperPwm::h_applyEnable(bool e)
{
    if (m_enableOut.is_connected())
        m_enableOut.write(e ? k_hardStop_Enable : k_hardStop_Disable);
}

void StepperPwm::h_irq_handler()
{
    __disable_irq();

    // Note: use PwmOutX and method pulse_width_us() for soft start implementation 
    //if (rotated_ && stepsCount_ == 3000) {
        //pwm.pulse_width_us(calcPulsePeriod(ppsFromRpm(1200, 200*16)));
    //}

    if (m_stepsCount++ >= m_stepsLimit)
    {
        h_stopAnyway();
    }
    __enable_irq();
}

void StepperPwm::h_irq_handler_static(uint32_t id, gpio_irq_event event) // static
{
    static_assert(sizeof(id) >= sizeof(void*));

    if (event == IRQ_RISE)
    {
        StepperPwm *self = (StepperPwm *)id;
        self->h_irq_handler();
    }
}


/*int AsyncStepper::readCount2() const 
{
    return TIM1->CNT;
}*/
