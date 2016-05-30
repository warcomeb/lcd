/******************************************************************************
 * Copyright (C) 2016 Marco Giammarini
 *
 * Authors:
 *  Marco Giammarini <m.giammarini@warcomeb.it>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/**
 * @file lcd/lcd.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief LCD .
 */

#ifndef __WARCOMEB_LCD_H
#define __WARCOMEB_LCD_H

#define WARCOMEB_LCD_LIBRARY_VERSION     "0.1"
#define WARCOMEB_LCD_LIBRARY_VERSION_M   0
#define WARCOMEB_LCD_LIBRARY_VERSION_m   1
#define WARCOMEB_LCD_LIBRARY_TIME        0

#include "libohiboard.h"

typedef enum _Lcd_PinMode
{
    LCD_PINMODE_4BIT,
    LCD_PINMODE_8BIT
} Lcd_PinMode;

/**
 *
 */
typedef struct _Lcd_Device
{
    Gpio_Pins rs;
    Gpio_Pins rw;
    Gpio_Pins enable;

    Gpio_Pins d0;
    Gpio_Pins d1;
    Gpio_Pins d2;
    Gpio_Pins d3;
    Gpio_Pins d4;
    Gpio_Pins d5;
    Gpio_Pins d6;
    Gpio_Pins d7;

    uint8_t columns;
    uint8_t rows;

    uint8_t display;
    uint8_t mode;
    uint8_t function;

    Lcd_PinMode pinMode;

    uint8_t basetime;

    uint8_t rowsOffsets[4];

} Lcd_Device;

void Lcd_init (Lcd_Device* dev);

void Lcd_write (Lcd_Device* dev, uint8_t ch);
void Lcd_writeString (Lcd_Device* dev, const uint8_t *data);

void Lcd_displayOn (Lcd_Device* dev);
void Lcd_displayOff (Lcd_Device* dev);

void Lcd_cursorOn (Lcd_Device* dev);
void Lcd_cursorOff (Lcd_Device* dev);

void Lcd_blinkOn (Lcd_Device* dev);
void Lcd_blinkOff (Lcd_Device* dev);

void Lcd_scrollDisplayLeft (Lcd_Device* dev);
void Lcd_scrollDisplayRight (Lcd_Device* dev);

void Lcd_leftToRight (Lcd_Device* dev);
void Lcd_rightToLeft (Lcd_Device* dev);

void Lcd_autoscrollOn (Lcd_Device* dev);
void Lcd_autoscrollOff (Lcd_Device* dev);

void Lcd_clear (Lcd_Device* dev);
void Lcd_home (Lcd_Device* dev);
void Lcd_setCursor (Lcd_Device* dev, uint8_t column, uint8_t row);

void Lcd_setRowsOffset (Lcd_Device* dev, uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3);


#endif /* __WARCOMEB_LCD_H */
