#pragma once

namespace _error
{
    enum _value
    {
        ok = 1,
        wrong_psw = -1,
        no_such_user = -2,
        already_login = -3,
        no_auth = -4,
    };

    std::string to_msg(_value val)
    {
        switch (val)
        {
        case ok:
            return "ok";
        case wrong_psw:
            return "wrong password";
        case no_such_user:
            return "no such user";



        default:
            return "";
        }
    }
}