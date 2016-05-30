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

#define LCD_DISPLAY_ON           0x04
#define LCD_DISPLAY_OFF          0x00

#define LCD_CURSOR_ON            0x02
#define LCD_CURSOR_OFF           0x00

#define LCD_BLINK_ON             0x01
#define LCD_BLINK_OFF            0x00

#define LCD_MODE_8BIT            0x10
#define LCD_MODE_4BIT            0x00

#define LCD_LINE_2               0x08
#define LCD_LINE_1               0x00

#define LCD_DOTS_5x10            0x04
#define LCD_DOTS_5x8             0x00

#define LCD_ENTRY_RIGHT          0x00
#define LCD_ENTRY_LEFT           0x02
#define LCD_SHIFT_INCREMENT      0x01
#define LCD_SHIFT_DECREMENT      0x00

#define LCD_MOVE_DISPLAY         0x08
#define LCD_MOVE_CURSOR          0x00
#define LCD_MOVE_RIGHT           0x04
#define LCD_MOVE_LEFT            0x00

#define LCD_ROW_OFFESET_0        0x00
#define LCD_ROW_OFFESET_1        0x40
#define LCD_ROW_OFFESET_2        0x14
#define LCD_ROW_OFFESET_3        0x54

/**
 *
 * @param delay Is the time delay in microseconds
 */
static void Lcd_delay (Lcd_Device* dev, uint16_t delay)
{
    uint32_t number = (uint32_t) ((uint32_t)(delay * 1000)) / dev->basetime;
    uint32_t i;

    for (i = 0; i < number; ++i) __asm ("nop");
}

static void Lcd_sendEnable (Lcd_Device* dev)
{
    Gpio_clear(dev->enable);
    Lcd_delay(dev,2);
    Gpio_set(dev->enable);
    Lcd_delay(dev,2);
    Gpio_clear(dev->enable);
    Lcd_delay(dev,100);
}

static void Lcd_write4 (Lcd_Device* dev, uint8_t value)
{
    ((value & 0x01) > 0) ? Gpio_set(dev->d0) : Gpio_clear(dev->d0);
    ((value & 0x02) > 0) ? Gpio_set(dev->d1) : Gpio_clear(dev->d1);
    ((value & 0x04) > 0) ? Gpio_set(dev->d2) : Gpio_clear(dev->d2);
    ((value & 0x08) > 0) ? Gpio_set(dev->d3) : Gpio_clear(dev->d3);

    Lcd_sendEnable(dev);
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

    Lcd_sendEnable(dev);
}

static void Lcd_sendCommand (Lcd_Device* dev, uint8_t value, bool write)
{
    if (write == TRUE)
        Gpio_set(dev->rs);
    else
        Gpio_clear(dev->rs);

//    if (dev->rw != GPIO_PINS_NONE)
//        Gpio_clear(dev->rw);

    if (dev->pinMode == LCD_PINMODE_8BIT)
    {
        Lcd_write8(dev,value);
    }
    else
    {
        Lcd_write4(dev,value>>4);
        Lcd_write4(dev,value);
    }

//    if (dev->rw != GPIO_PINS_NONE)
//        Gpio_set(dev->rw);
}

void Lcd_init (Lcd_Device* dev)
{
    if (dev->basetime == 0) dev->basetime = 20;

    /* Enable all pins */
    if (dev->pinMode == LCD_PINMODE_4BIT)
    {
        Gpio_config(dev->d0,GPIO_PINS_OUTPUT);
        Gpio_config(dev->d1,GPIO_PINS_OUTPUT);
        Gpio_config(dev->d2,GPIO_PINS_OUTPUT);
        Gpio_config(dev->d3,GPIO_PINS_OUTPUT);
        dev->d4 = GPIO_PINS_NONE;
        dev->d5 = GPIO_PINS_NONE;
        dev->d6 = GPIO_PINS_NONE;
        dev->d7 = GPIO_PINS_NONE;

        dev->function |= LCD_MODE_4BIT;
    }
    else
    {
        Gpio_config(dev->d0,GPIO_PINS_OUTPUT);
        Gpio_config(dev->d1,GPIO_PINS_OUTPUT);
        Gpio_config(dev->d2,GPIO_PINS_OUTPUT);
        Gpio_config(dev->d3,GPIO_PINS_OUTPUT);
        Gpio_config(dev->d4,GPIO_PINS_OUTPUT);
        Gpio_config(dev->d5,GPIO_PINS_OUTPUT);
        Gpio_config(dev->d6,GPIO_PINS_OUTPUT);
        Gpio_config(dev->d7,GPIO_PINS_OUTPUT);

        dev->function |= LCD_MODE_8BIT;
    }

    if (dev->rw != GPIO_PINS_NONE)
        Gpio_config(dev->rw,GPIO_PINS_OUTPUT);

    Gpio_config(dev->enable,GPIO_PINS_OUTPUT);
    Gpio_config(dev->rs,GPIO_PINS_OUTPUT);

    Lcd_setRowsOffset(dev,LCD_ROW_OFFESET_0,LCD_ROW_OFFESET_1,LCD_ROW_OFFESET_2,LCD_ROW_OFFESET_3);

    if (dev->rows == 1) dev->function |= LCD_LINE_1;
    else dev->function |= LCD_LINE_2;

    dev->function |= LCD_DOTS_5x8;

    /* according to datasheet, we need at least 30ms after power rises above 2.7V */
    Lcd_delay(dev,50000);

    if (dev->pinMode == LCD_PINMODE_4BIT)
    {
        /* BF cannot be checked before this instruction */
        Lcd_write4(dev,0x03);
        Lcd_delay(dev,5000);

        /* BF cannot be checked before this instruction */
        Lcd_write4(dev,0x03);
        Lcd_delay(dev,5000);

        /* BF cannot be checked before this instruction */
        Lcd_write4(dev,0x03);
        Lcd_delay(dev,150);

        /* BF can be checked before this instruction */
        Lcd_write4(dev,0x02);
    }
    else
    {
        /* BF cannot be checked before this instruction */
        Lcd_sendCommand(dev,LCD_CMD_FUNCTION_SET | dev->function,FALSE);
        Lcd_delay(dev,5000);

        /* BF cannot be checked before this instruction */
        Lcd_sendCommand(dev,LCD_CMD_FUNCTION_SET | dev->function,FALSE);
        Lcd_delay(dev,150);

        /* BF can be checked before this instruction */
        Lcd_sendCommand(dev,LCD_CMD_FUNCTION_SET | dev->function,FALSE);
    }

    dev->display = LCD_DISPLAY_ON |
                   LCD_CURSOR_OFF |
                   LCD_BLINK_OFF;
    Lcd_displayOn(dev);
    Lcd_delay(dev,4000);

    /* The delay is included into function clear */
    Lcd_clear(dev);

    /* Romance languages */
    dev->mode = LCD_ENTRY_LEFT |
                LCD_SHIFT_DECREMENT;
    Lcd_sendCommand(dev,LCD_CMD_ENTRY_MODE_SET | dev->mode,FALSE);
}

void Lcd_setRowsOffset (Lcd_Device* dev, uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3)
{
    dev->rowsOffsets[0] = row0;
    dev->rowsOffsets[1] = row1;
    dev->rowsOffsets[2] = row2;
    dev->rowsOffsets[3] = row3;
}

/**
 *
 * @param period Is the microcontroller period in nanoseconds
 */
void Lcd_setDelayBasetime (Lcd_Device* dev, uint8_t period)
{
    dev->basetime = period;
}

void Lcd_setCursor (Lcd_Device* dev, uint8_t column, uint8_t row)
{

    if (row > dev->rows) row = dev->rows - 1;
    if (column > dev->columns) column = dev->columns - 1;

    Lcd_sendCommand(dev,LCD_CMD_SET_DDRAM_ADDR | (column + dev->rowsOffsets[row]),FALSE);
}

void Lcd_write (Lcd_Device* dev, uint8_t ch)
{
    Lcd_sendCommand(dev,ch,TRUE);
}

void Lcd_writeString (Lcd_Device* dev, const uint8_t *data)
{
    if (data)
    {
        while (*data)
        {
            Lcd_sendCommand(dev,*data++,TRUE);
        }
    }
}

void Lcd_displayOn (Lcd_Device* dev)
{
    dev->display |= LCD_DISPLAY_ON;
    Lcd_sendCommand(dev,LCD_CMD_DISPLAY_CONTROL | dev->display,FALSE);
}

void Lcd_displayOff (Lcd_Device* dev)
{
    dev->display &= ~LCD_DISPLAY_ON;
    Lcd_sendCommand(dev,LCD_CMD_DISPLAY_CONTROL | dev->display,FALSE);
}

void Lcd_cursorOn (Lcd_Device* dev)
{
    dev->display |= LCD_CURSOR_ON;
    Lcd_sendCommand(dev,LCD_CMD_DISPLAY_CONTROL | dev->display,FALSE);
}

void Lcd_cursorOff (Lcd_Device* dev)
{
    dev->display &= ~LCD_CURSOR_ON;
    Lcd_sendCommand(dev,LCD_CMD_DISPLAY_CONTROL | dev->display,FALSE);
}

void Lcd_blinkOn (Lcd_Device* dev)
{
    dev->display |= LCD_BLINK_ON;
    Lcd_sendCommand(dev,LCD_CMD_DISPLAY_CONTROL | dev->display,FALSE);
}

void Lcd_blinkOff (Lcd_Device* dev)
{
    dev->display &= ~LCD_BLINK_ON;
    Lcd_sendCommand(dev,LCD_CMD_DISPLAY_CONTROL | dev->display,FALSE);
}

void Lcd_scrollDisplayLeft (Lcd_Device* dev)
{
    Lcd_sendCommand(
        dev,
        LCD_CMD_CURSOR_SHIFT | LCD_MOVE_DISPLAY | LCD_MOVE_LEFT,
        FALSE
    );
}

void Lcd_scrollDisplayRight (Lcd_Device* dev)
{
    Lcd_sendCommand(
        dev,
        LCD_CMD_CURSOR_SHIFT | LCD_MOVE_DISPLAY | LCD_MOVE_RIGHT,
        FALSE
    );
}

void Lcd_leftToRight (Lcd_Device* dev)
{
    dev->display |= LCD_ENTRY_LEFT;
    Lcd_sendCommand(dev,LCD_CMD_ENTRY_MODE_SET | dev->display,FALSE);
}

void Lcd_rightToLeft (Lcd_Device* dev)
{
    dev->display &= ~LCD_ENTRY_LEFT;
    Lcd_sendCommand(dev,LCD_CMD_ENTRY_MODE_SET | dev->display,FALSE);
}

void Lcd_autoscrollOn (Lcd_Device* dev)
{
    dev->display |= LCD_SHIFT_INCREMENT;
    Lcd_sendCommand(dev,LCD_CMD_ENTRY_MODE_SET | dev->display,FALSE);
}

void Lcd_autoscrollOff (Lcd_Device* dev)
{
    dev->display &= ~LCD_SHIFT_INCREMENT;
    Lcd_sendCommand(dev,LCD_CMD_ENTRY_MODE_SET | dev->display,FALSE);
}

void Lcd_clear (Lcd_Device* dev)
{
    Lcd_sendCommand(dev,LCD_CMD_CLEAR_DISPLAY,FALSE);
    Lcd_delay(dev,2000);
}

void Lcd_home (Lcd_Device* dev)
{
    Lcd_sendCommand(dev,LCD_CMD_RETURN_HOME,FALSE);
    Lcd_delay(dev,2000);
}

