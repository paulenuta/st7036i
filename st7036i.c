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
#include "util.h"
#include "i2c.h"
#include "st7036i.h"

uint8 st7036_Cursor = FALSE;
uint8 st7036_Blink = TRUE;

void disp_init()
{
    i2c_send_start();
    i2c_send(0x78); // 
    i2c_send(0x00); // command entry
    i2c_send(0x38); // function set: 8bit, 2 lines 0b00111000
    delay_ms(2);
    i2c_send(0x39); // function set instruction table 1
    delay_ms(2);
    i2c_send(0x14); // bias  14
    i2c_send(0x78); // contrast  78
    i2c_send(0x5e); //
    i2c_send(0x6d); //
    i2c_send(0x0c);
    i2c_send(0x01);
    i2c_send(0x06);
    delay_ms(2);
    i2c_send_stop();
    delay_ms(10);

    disp_on();
}

void disp_on()
{
    uint8 byte = 0x0C;

    if (st7036_Cursor == TRUE)
        byte |= 0b10;
    if (st7036_Blink == TRUE)
        byte |= 0b1;

    i2c_send_start();
    i2c_send(0x78); 
    i2c_send(0x00);
    i2c_send(byte);    
    i2c_send_stop();
    delay_ms(10);
}

void disp_off()
{
    i2c_send_start();
    i2c_send(0x78); 
    i2c_send(0x00);
    i2c_send(0x08);    
    i2c_send_stop();
    delay_ms(10);
}

void disp_clear()
{
    i2c_send_start();
    i2c_send(0x78); 
    i2c_send(0x00);
    i2c_send(0x01);    
    i2c_send_stop();
    delay_ms(10);
}

void disp_home()
{
    i2c_send_start();
    i2c_send(0x78); 
    i2c_send(0x00);
    i2c_send(0x02);    
    i2c_send_stop();
    delay_ms(10);
}

void disp_cursor(uint8 status)
{
    if (status == 0)
        st7036_Cursor = FALSE;
    else
        st7036_Cursor = TRUE;
}

void disp_blink(uint8 status)
{
    if (status == 0)
        st7036_Blink = FALSE;
    else
        st7036_Blink = TRUE;
}

void disp_setpos(uint8 row, uint8 col)
{
    i2c_send_start();
    i2c_send(0x78);
    i2c_send(0x00);
    if (row == 0) { i2c_send(0x80 + col); }
    if (row == 1) { i2c_send(0xc0 + col); }
    i2c_send_stop();
    delay_ms(10);
}

void disp_putc(uint8 byte)
{
    i2c_send_start();    
    i2c_send(0x78);
    i2c_send(0x40);
    i2c_send(byte);
    i2c_send_stop();
    delay_ms(10);
}

void disp_puts(uint8 *array)
{
    uint8 i=0;
    uint8 byte;
    
    delay_ms(2);
    i2c_send_start();    
    i2c_send(0x78);
    i2c_send(0x40);
    do {
        byte = array[i];
        if (byte == 0)
            break;
        else {
            i2c_send(byte);            
        }
        ++i;
    } while (i < 20);
    i2c_send_stop();
    delay_ms(10);
}
