/**
 * \file doormodule.cpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief door module class
 */

#include "doormodule.hpp"

#include <sstream>

#include "config/xmlconfig.hpp"
#include "tools/log.hpp"
#include "hardware/device/led.hpp"
#include "hardware/device/buzzer.hpp"
#include "hardware/device/button.hpp"
#include "hardware/device/relay.hpp"
#include "exception/moduleexception.hpp"

template <typename T>
static T* loadOptionnalDevice(const std::string& name, IHWManager& HWManager)
{
    T* ptr;

    if (name == "none")
        return (nullptr);
    else if ((ptr = dynamic_cast<T*>(HWManager.getDevice(name))))
        return (ptr);
    else
        throw (ModuleException("could not retrieve device \'" + name + '\''));
}

DoorModule::DoorModule(ICore& core, const std::string& name)
:   _core(core),
    _hwmanager(core.getHWManager()),
    _name(name),
    _doorRelay(nullptr),
    _doorButton(nullptr),
    _doorSensor(nullptr),
    _grantedLed(nullptr),
    _deniedLed(nullptr),
    _buzzer(nullptr)
{}

const std::string& DoorModule::getName() const
{
    return (_name);
}

IModule::ModuleType DoorModule::getType() const
{
    return (ModuleType::Door);
}

void DoorModule::serialize(ptree& node)
{
    XmlConfig   conf(_config.doorConf, _doorConfig);

    conf.serialize();

    node.put<std::string>("doorconf", _config.doorConf);
    node.put<std::string>("doorRelay", _config.doorRelay);
    node.put<std::string>("doorButton", _config.doorButton);
    node.put<std::string>("doorSensor", _config.doorSensor);
    node.put<std::string>("grantedLed", _config.grantedLed);
    node.put<std::string>("deniedLed", _config.deniedLed);
    node.put<std::string>("buzzer", _config.buzzer);
}

void DoorModule::deserialize(const ptree& node)
{
    _config.doorConf = node.get<std::string>("doorconf");
    _config.doorRelay = node.get<std::string>("doorRelay", "none");
    _config.doorButton = node.get<std::string>("doorButton", "none");
    _config.doorSensor = node.get<std::string>("doorSensor", "none");
    _config.grantedLed = node.get<std::string>("grantedLed", "none");
    _config.deniedLed = node.get<std::string>("deniedLed", "none");
    _config.buzzer = node.get<std::string>("buzzer", "none");

    _doorRelay = loadOptionnalDevice<Relay>(_config.doorRelay, _hwmanager);
    _doorButton = loadOptionnalDevice<Button>(_config.doorButton, _hwmanager);
    _doorSensor = loadOptionnalDevice<Button>(_config.doorSensor, _hwmanager);
    _grantedLed = loadOptionnalDevice<Led>(_config.grantedLed, _hwmanager);
    _deniedLed = loadOptionnalDevice<Led>(_config.deniedLed, _hwmanager);
    _buzzer = loadOptionnalDevice<Buzzer>(_config.buzzer, _hwmanager);

    XmlConfig   conf(_config.doorConf, _doorConfig);
    conf.deserialize();
}

bool DoorModule::isAuthRequired() const
{
    return (true); // FIXME
}

void DoorModule::open()
{
    if (_grantedLed)
        _grantedLed->blink(); // DEBUG
    LOG() << "DOOR OPENED !";
    if (_doorRelay)
        _doorRelay->open();
    else
        LOG() << "No relay to open !";
}

bool DoorModule::isOpen() const
{
    return (_doorSensor->isPressed());
}

void DoorModule::alarm()
{
    if (_buzzer)
        _buzzer->beep(10000, 500);
    else
        LOG() << "There's no buzzer to buzz.";
    _core.getModuleProtocol().logMessage("Door should be closed !!");
}
