/**
 * @file cmderror.h
 * @brief CmdError class definition
 * @author Enrique Walter Philippeaux
 * @date 2022
 */

#ifndef CMDERROR_H
#define CMDERROR_H

class CmdError
{
    typedef void (CmdError::*bool_type)() const;
    void safeBoolHelper() const {}

public:
    enum Code
    {
        Ok,
        CommandError,
        NullBuffer,
        BufferTooShort,
        DLCError,
        DLCTooLong,
        DataError,
        ChecksumError
    };

    CmdError() {}
    CmdError(Code c) : _code(c) {}

    Code code() const
    {
        return _code;
    }

    // Compare with CmdError
    friend bool operator==(const CmdError &lhs,
                           const CmdError &rhs)
    {
        return lhs._code == rhs._code;
    }
    friend bool operator!=(const CmdError &lhs,
                           const CmdError &rhs)
    {
        return lhs._code != rhs._code;
    }

    // Compare with Code
    friend bool operator==(const CmdError &lhs, Code rhs)
    {
        return lhs._code == rhs;
    }
    friend bool operator==(Code lhs, const CmdError &rhs)
    {
        return lhs == rhs._code;
    }
    friend bool operator!=(const CmdError &lhs, Code rhs)
    {
        return lhs._code != rhs;
    }
    friend bool operator!=(Code lhs, const CmdError &rhs)
    {
        return lhs != rhs._code;
    }

    // Behaves like a bool
    operator bool_type() const
    {
        return _code != Ok ? &CmdError::safeBoolHelper : 0;
    }
    friend bool operator==(bool value, const CmdError &err)
    {
        return static_cast<bool>(err) == value;
    }
    friend bool operator==(const CmdError &err, bool value)
    {
        return static_cast<bool>(err) == value;
    }
    friend bool operator!=(bool value, const CmdError &err)
    {
        return static_cast<bool>(err) != value;
    }
    friend bool operator!=(const CmdError &err, bool value)
    {
        return static_cast<bool>(err) != value;
    }

    const char *c_str() const
    {
        switch (_code)
        {
        case Ok:
            return "Ok";
        case CommandError:
            return "CommandError";
        case NullBuffer:
            return "NullBuffer";
        case BufferTooShort:
            return "BufferTooShort";
        case DLCError:
            return "DLCError";
        case DLCTooLong:
            return "DLCTooLong";
        case DataError:
            return "DataError";
        case ChecksumError:
            return "ChecksumError";
        default:
            return "???";
        }
    }

private:
    Code _code;
};


#endif