#include "pwmout_x.h"


extern TIM_HandleTypeDef TimHandle;

PwmOutX::PwmOutX(PinName pin) : mbed::PwmOut(pin)
{

}

PwmOutX::~PwmOutX()
{

}

static unsigned int timChannel(pwmout_t* obj)
{
    unsigned int channel = 0;

    switch (obj->channel) {
    case 1:
        channel = TIM_CHANNEL_1;
        break;
    case 2:
        channel = TIM_CHANNEL_2;
        break;
    case 3:
        channel = TIM_CHANNEL_3;
        break;
    case 4:
        channel = TIM_CHANNEL_4;
        break;
    default:
        break;
    }

    return channel;
}

static unsigned int timInterrupt(pwmout_t* obj)
{
    unsigned int channel = 0;

    switch (obj->channel) {
    case 1:
        channel = TIM_IT_CC1;
        break;
    case 2:
        channel = TIM_IT_CC2;
        break;
    case 3:
        channel = TIM_IT_CC3;
        break;
    case 4:
        channel = TIM_IT_CC4;
        break;
    default:
        break;
    }

    return channel;
}

/* Convert STM32 Cube HAL channel to LL channel */
uint32_t myTIM_ChannelConvert_HAL2LL(uint32_t channel, pwmout_t *obj)
{
#if !defined(PWMOUT_INVERTED_NOT_SUPPORTED)
    if (obj->inverted) {
        switch (channel) {
            case TIM_CHANNEL_1  :
                return LL_TIM_CHANNEL_CH1N;
            case TIM_CHANNEL_2  :
                return LL_TIM_CHANNEL_CH2N;
            case TIM_CHANNEL_3  :
                return LL_TIM_CHANNEL_CH3N;
#if defined(LL_TIM_CHANNEL_CH4N)
            case TIM_CHANNEL_4  :
                return LL_TIM_CHANNEL_CH4N;
#endif
            default : /* Optional */
                return 0;
        }
    } else
#endif
    {
        switch (channel) {
            case TIM_CHANNEL_1  :
                return LL_TIM_CHANNEL_CH1;
            case TIM_CHANNEL_2  :
                return LL_TIM_CHANNEL_CH2;
            case TIM_CHANNEL_3  :
                return LL_TIM_CHANNEL_CH3;
            case TIM_CHANNEL_4  :
                return LL_TIM_CHANNEL_CH4;
            default : /* Optional */
                return 0;
        }
    }
}

static void stopInInterrupt(TIM_TypeDef* ptim, uint32_t timInterrupt)
{
    ptim->DIER &= ~timInterrupt;
}

static void startInInterrupt(TIM_TypeDef* ptim, uint32_t timInterrupt)
{
    ptim->DIER |= timInterrupt;
}

void PwmOutX::stop()
{

    TIM_TypeDef* ptim = (TIM_TypeDef *)(_pwm.pwm);

    TIM_CCxChannelCmd(ptim, timChannel(&_pwm), TIM_CCx_DISABLE);

/*
    _pwm.pulse = 0;
    TIM_TypeDef* ptim = (TIM_TypeDef *)(_pwm.pwm);

    TimHandle.Instance = (TIM_TypeDef *)(_pwm.pwm);

*/

    //HAL_TIM_PWM_Stop(&TimHandle, timChannel(&_pwm));
}

void PwmOutX::start()
{
    TIM_TypeDef* ptim = (TIM_TypeDef *)(_pwm.pwm);

    //ptim->DIER 


    TIM_CCxChannelCmd(ptim, timChannel(&_pwm), TIM_CCx_ENABLE);
/*
    _pwm.pulse = 0;
    TIM_TypeDef* ptim = (TIM_TypeDef *)(_pwm.pwm);

    TimHandle.Instance = (TIM_TypeDef *)(_pwm.pwm);

*/

    //HAL_TIM_PWM_Stop(&TimHandle, timChannel(&_pwm));
}

static void tim_disable(TIM_TypeDef* obj)
{
    if ((obj->CCER & TIM_CCER_CCxE_MASK) == 0U)
    { 
        if((obj->CCER & TIM_CCER_CCxNE_MASK) == 0U)
        {
            obj->CR1 &= ~(TIM_CR1_CEN);
        }
    }
}

static void tim_enable(TIM_TypeDef* obj)
{
    obj->CR1|=(TIM_CR1_CEN);
}

void PwmOutX::pulse_width_us(uint32_t w)
{
    TIM_TypeDef* ptim = (TIM_TypeDef *)(_pwm.pwm);
    tim_disable(ptim);

    auto channel = timChannel(&_pwm);
    _pwm.period = w;

    ptim->ARR = w;

    //TIM_Base_SetConfig(htim->Instance, &htim->Init);
  
    //_pwm.pulse = w;
    //auto pulse = w / _pwm.prescaler;

    /*
    if (LL_TIM_CC_IsEnabledChannel(ptim, myTIM_ChannelConvert_HAL2LL(channel, &_pwm)) == 0) {
        // If channel is not enabled, proceed to channel configuration
        //if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel) != HAL_OK) {
        //    error("Cannot initialize PWM\n");
        //}
        //if (false)
        switch (_pwm.channel) {
            case 1: ptim->CCR1 = pulse; break;
            case 2: ptim->CCR2 = pulse; break;
            case 3: ptim->CCR3 = pulse; break;
            case 4: ptim->CCR4 = pulse; break;
            default: break;
        }

        //ptim->CCR1 = pulse;
        //TIMx->CCR1 = OC_Config->Pulse;
    }
    else {
        switch (_pwm.channel) {
            case 1: *(__IO uint32_t *)(&(ptim->CCR1) + ((channel) >> 2U)) = pulse;; break;
            case 2: *(__IO uint32_t *)(&(ptim->CCR2) + ((channel) >> 2U)) = pulse;; break;
            case 3: *(__IO uint32_t *)(&(ptim->CCR3) + ((channel) >> 2U)) = pulse;; break;
            case 4: *(__IO uint32_t *)(&(ptim->CCR4) + ((channel) >> 2U)) = pulse;; break;
            default: break;
        }
        // *(__IO uint32_t *)(&(ptim->CCR1) + ((channel) >> 2U)) = pulse;
    }

    //start();



    if ((ptim == TIM1) != RESET) {
        ptim->BDTR|=(TIM_BDTR_MOE); // __HAL_TIM_MOE_ENABLE
    } 

    ptim->CR1|=(TIM_CR1_CEN); //__HAL_TIM_ENABLE
    */

    tim_enable(ptim);
}

void PwmOutX::set_reprtition(uint32_t rep)
{
    
    TIM_TypeDef* ptim = (TIM_TypeDef *)(_pwm.pwm);
    // repetition counter
    tim_disable(ptim);
    ptim->RCR = rep;
    tim_enable(ptim);
}

uint32_t PwmOutX::count() const
{
    TIM_TypeDef* ptim = (TIM_TypeDef *)(_pwm.pwm);
    return ptim->CNT;
}



