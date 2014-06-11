/**
 * \file authtestmodule.hpp
 * \author Thibault Schueller <ryp.sqrt@gmail.com>
 * \brief simple auth module
 */

#ifndef AUTHTESTMODULE_HPP
#define AUTHTESTMODULE_HPP

#include "modules/iauthmodule.hpp"
#include "tools/bufferutils.hpp"
#include "authenticator.hpp"

class AuthTestModule : public IAuthModule
{
public:
    typedef std::vector<Byte> CardId;

public:
    explicit AuthTestModule(ICore& core, const std::string& name);
    ~AuthTestModule() = default;

    AuthTestModule(const AuthTestModule& other) = delete;
    AuthTestModule& operator=(const AuthTestModule& other) = delete;

public:
    virtual const std::string&  getName() const override;
    virtual ModuleType          getType() const override;
    virtual void                serialize(ptree& node) override;
    virtual void                deserialize(const ptree& node) override;
    virtual void                authenticate(const AuthRequest& ar) override;

private:
    ICore&                  _core;
    Authenticator           _auth;
    const std::string       _name;
};

#endif // AUTHTESTMODULE_HPP