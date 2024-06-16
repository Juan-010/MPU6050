/**
 * @file cmdlib.h
 * @brief Libreria para el manejo de paquetes de datos por UART.
 * @author Enrique Walter Philippeaux
 * @date 2022
 */

#ifndef CMDLIB_H
#define CMDLIB_H

#include "cmderror.h"
#include <stdint.h>

#define CMDLIB_MAX_DATA_LEN 1024

class Cmd
{ // Clase base para los comandos
public:
    Cmd();

    Cmd(uint8_t cmd, uint16_t dlc, uint8_t *data, uint8_t comp_data);

    Cmd(const uint8_t *buffer, uint16_t len); // Constructor con buffer

    Cmd(uint8_t cmd, const char *str, uint8_t comp_data); // Constructor con string

    ~Cmd();
    
    Cmd(const Cmd &c) { *this = c; } // Constructor de copia
    Cmd &operator=(const Cmd &c);    // Operador de asignacion

    CmdError serialize(uint8_t **buffer, uint16_t &len);

    
    uint8_t genChksum(); //!< Genera el checksum

    uint8_t getCmd() { return __cmd; }                  //!< Devuelve el comando
    uint16_t getDlc() { return __dlc; }                 //!< Devuelve la longitud de los datos
    uint8_t *getData() { return __data; }               //!< Devuelve un puntero a los datos
    uint8_t getCompData() { return __comp_data; }       //!< Devuelve el byte de informacion complementaria
    uint8_t getChksum() { return __chksum; } //!< Devuelve el checksum
    CmdError getResult() { return __error; }            //!< Devuelve el resultado

private:
    uint8_t __cmd;               //!< Comando
    uint16_t __dlc;              //!< Longitud de los datos
    uint8_t *__data;             //!< Puntero a los datos
    uint8_t __comp_data;         //!< Byte de Informacion complementaria
    uint8_t __chksum;            //!< Checksum (CRC8)
    CmdError __error;            //!< Error
};

#endif