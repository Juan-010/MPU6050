#include "cmdlib.h"
#include <cstring>

Cmd::Cmd()
{
    __cmd = 0;
    __dlc = 0;
    __data = nullptr;
    __comp_data = 0;
    __chksum = 0;
}

Cmd::Cmd(uint8_t cmd, uint16_t dlc, uint8_t *data, uint8_t comp_data) // Constructor con parametros
{
    if (dlc > CMDLIB_MAX_DATA_LEN)
    {
        __cmd = 0;
        __dlc = 0;
        __data = nullptr;
        __comp_data = 0;
        __chksum = 0;
        __error = CmdError::DLCTooLong;
        return;
    }
    __cmd = cmd;
    __dlc = dlc;
    __data = new uint8_t[dlc];
    memcpy(__data, data, dlc);
    __comp_data = comp_data;
    __chksum = genChksum();
}

Cmd::Cmd(const uint8_t *buffer, uint16_t len) // Constructor con buffer
{
    if (len < 4)
    {
        __cmd = 0;
        __dlc = 0;
        __data = nullptr;
        __comp_data = 0;
        __chksum = 0;
        __error = CmdError::BufferTooShort;
        return;
    }
    __cmd = buffer[0];
    __dlc = (buffer[1] << 8) | buffer[2];
    if (__dlc > CMDLIB_MAX_DATA_LEN)
    {
        __cmd = 0;
        __dlc = 0;
        __data = nullptr;
        __comp_data = 0;
        __chksum = 0;
        __error = CmdError::DLCTooLong;
        return;
    }
    __data = new uint8_t[__dlc];
    memcpy(__data, buffer + 3, __dlc);
    __comp_data = buffer[3 + __dlc];
    __chksum = buffer[3 + __dlc + 1];
    if (__chksum != genChksum())
    {
        __error = CmdError::ChecksumError;
        return;
    }
    __error = CmdError::Ok;
}

Cmd::Cmd(uint8_t cmd, const char *str, uint8_t comp_data) {
    __cmd = cmd;
    __dlc = strlen(str) + 1;
    if (__dlc > CMDLIB_MAX_DATA_LEN)
    {
        __cmd = 0;
        __dlc = 0;
        __data = nullptr;
        __comp_data = 0;
        __chksum = 0;
        __error = CmdError::DLCTooLong;
        return;
    }
    __data = new uint8_t[__dlc];
    memcpy(__data, str, __dlc);
    __comp_data = comp_data;
    __chksum = genChksum();
}

Cmd::~Cmd()
{
    if (__data != nullptr)
    {
        delete[] __data;
        __data = nullptr;
    }
} // Destructor

Cmd &Cmd::operator=(const Cmd &c) {
    __cmd = c.__cmd;
    __dlc = c.__dlc;
    if (__data != nullptr)
    {
        delete[] __data;
        __data = nullptr;
    }
    if (c.__data != nullptr)
    {
        __data = new uint8_t[__dlc];
        memcpy(__data, c.__data, __dlc);
    }
    __comp_data = c.__comp_data;
    __chksum = c.__chksum;
    __error = c.__error;
    return *this;
}

CmdError Cmd::serialize(uint8_t **buffer, uint16_t &len) {
    len = __dlc + 5;
    *buffer = new uint8_t[len];
    *buffer[0] = __cmd;
    *buffer[1] = (__dlc >> 8) & 0xFF;
    *buffer[2] = __dlc & 0xFF;
    memcpy(*buffer + 3, __data, __dlc);
    *buffer[3 + __dlc] = __comp_data;
    *buffer[3 + __dlc + 1] = genChksum();
    return CmdError::Ok;
}

uint8_t Cmd::genChksum()
{
    __chksum ^= __cmd;
    __chksum ^= (__dlc >> 8) & 0xFF;
    __chksum ^= __dlc & 0xFF;
    if (__data != nullptr)
        for (uint16_t i = 0; i < __dlc - 1; i++)
            __chksum ^= __data[i];
    __chksum ^= __comp_data;

    return __chksum;
}