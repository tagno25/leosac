/**
 * \file igpiolistener.hpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief interface for gpio event listeners
 */

#ifndef IGPIOLISTENER_HPP
#define IGPIOLISTENER_HPP

#include "gpio.hpp"

class IGPIOListener
{
public:
    virtual ~IGPIOListener() {}
    virtual void    notify(int gpioNo) = 0;
};

#endif // IGPIOLISTENER_HPP
