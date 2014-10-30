/*
The MIT License (MIT)

Copyright (c) 2014 Jordan Wright

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <avr/io.h>
#include <avr/iom168.h>

#include "types.h"
#include "i2c.h"

/* Generic support library for I2C */
/* Support library for the ST7036i chipset using a 2x20 display */

void i2c_init()
{
    // Set clock rate generator to 100kHz with master clock at 8 MHz
    // SCL freq = master clock / (16 + 2(TWBR)*(Prescaler))
    TWBR = 8;
    TWSR |= (0 << TWPS1) | (1 << TWPS0); // Prescaler of 4
}

void i2c_send(uint8 byte)
{
    TWDR = byte;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}


void i2c_send_start()
{
    // Enable TWI system, send start condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT))) ;    
}

void i2c_send_stop()
{
    // Enable TWI system, send stop condition
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}
