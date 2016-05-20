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

#include "lcd.h"

#define LCD_CMD_CLEAR_DISPLAY    0x01
#define LCD_CMD_RETURN_HOME      0x02
#define LCD_CMD_ENTRY_MODE_SET   0x04
#define LCD_CMD_DISPLAY_CONTROL  0x08
#define LCD_CMD_CURSOR_SHIFT     0x10
#define LCD_CMD_FUNCTION_SET     0x30
#define LCD_CMD_SET_CGRAM_ADDR   0x40
#define LCD_CMD_SET_DDRAM_ADDR   0x80

#define LCD_ROW_OFFESET_0        0x00
#define LCD_ROW_OFFESET_1        0x40
#define LCD_ROW_OFFESET_2        0x14
#define LCD_ROW_OFFESET_3        0x54

static void Lcd_write4 (Lcd_Device* dev, uint8_t value)
{
    ((value & 0x01) > 0) ? Gpio_set(dev->d0) : Gpio_clear(dev->d0);
    ((value & 0x02) > 0) ? Gpio_set(dev->d1) : Gpio_clear(dev->d1);
    ((value & 0x04) > 0) ? Gpio_set(dev->d2) : Gpio_clear(dev->d2);
    ((value & 0x08) > 0) ? Gpio_set(dev->d3) : Gpio_clear(dev->d3);

    // TODO: ENABLE
}

static void Lcd_write8 (Lcd_Device* dev, uint8_t value)
{
    ((value & 0x01) > 0) ? Gpio_set(dev->d0) : Gpio_clear(dev->d0);
    ((value & 0x02) > 0) ? Gpio_set(dev->d1) : Gpio_clear(dev->d1);
    ((value & 0x04) > 0) ? Gpio_set(dev->d2) : Gpio_clear(dev->d2);
    ((value & 0x08) > 0) ? Gpio_set(dev->d3) : Gpio_clear(dev->d3);
    ((value & 0x10) > 0) ? Gpio_set(dev->d4) : Gpio_clear(dev->d4);
    ((value & 0x20) > 0) ? Gpio_set(dev->d5) : Gpio_clear(dev->d5);
    ((value & 0x40) > 0) ? Gpio_set(dev->d6) : Gpio_clear(dev->d6);
    ((value & 0x80) > 0) ? Gpio_set(dev->d7) : Gpio_clear(dev->d7);

    // TODO: ENABLE
}

static void Lcd_sendCommand (Lcd_Device* dev, uint8_t value, bool write)
{
    if (write == TRUE)
        Gpio_set(dev->rs);
    else
        Gpio_clear(dev->rs);

    if (dev->rw != GPIO_PINS_NONE)
        Gpio_clear(dev->rw);

    if (dev->mode == LCD_MODE_8BIT)
    {
        Lcd_write8(dev,value);
    }
    else
    {
        Lcd_write4(dev,value>>4);
        Lcd_write4(dev,value);
    }

    if (dev->rw != GPIO_PINS_NONE)
        Gpio_set(dev->rw);
}


void Lcd_init (Lcd_Device* dev)
{

    Lcd_setRowsOffset(dev,LCD_ROW_OFFESET_0,LCD_ROW_OFFESET_1,LCD_ROW_OFFESET_2,LCD_ROW_OFFESET_3);
}

void Lcd_setRowsOffset (Lcd_Device* dev, uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3)
{
    dev->rowsOffsets[0] = row0;
    dev->rowsOffsets[1] = row1;
    dev->rowsOffsets[2] = row2;
    dev->rowsOffsets[3] = row3;
}

void Lcd_setCursor (Lcd_Device* dev, uint8_t column, uint8_t row)
{

    if (row > dev->rows) row = dev->rows - 1;
    if (column > dev->columns) column = dev->columns - 1;

    Lcd_sendCommand(dev,LCD_CMD_SET_DDRAM_ADDR | (column + offsets[row]),FALSE);
}
