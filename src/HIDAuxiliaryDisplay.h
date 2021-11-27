/*
  HIDAuxiliaryDisplay.h

  Copyright (c) 2021, Aleksandr Bratchik
  Original code (pre-library): Copyright (c) 2020, Aleksandr Bratchik

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef HIDAUXDISPLAY_h
#define HIDAUXDISPLAY_h

#define _USING_HID



#include "HID/HID.h"
#include "WString.h"


#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else
// Display params flags
#define HID_AUXD_ASCII_CHARSET      0x80 // Screen supports
#define HID_AUXD_DATAREAD_BACK      0x40 // Character Report can be read back when set
#define HID_AUXD_FONTREAD_BACK      0x20 // Font Report can be read back when set

// Display control flags
#define HID_AUXD_CLEAR              0x80 // Clear Display command
#define HID_AUXD_ENABLE             0x40 // Display enable
#define HID_AUXD_SSENABLE           0x20 // Screen Saver enable
#define HID_AUXD_VSCROLL            0x10 // Vertical Scroll
#define HID_AUXD_HSCROLL            0x08 // Horizintal Scroll

// Cursor control flags
#define HID_ADCC_PIXELPOS         0x80 // Cursor Pixel Positioning
#define HID_ADCC_INCREMENT        0x40 // Cursor Mode = Increment
#define HID_ADCC_ENABLE           0x20 // Cursor Enable
#define HID_ADCC_BLINK            0x10 // Cursor Blink 

//================================================================================
typedef struct
{
  uint8_t rows;     // 0x35
  uint8_t cols;     // 0x36
  uint8_t chrw;     // 0x3d char width 
  uint8_t chrh;     // 0x3e char height
  uint8_t flags;    // 0x21|0x22|0x29|5 unused
} HIDDisplayParams;


typedef struct
{
  uint8_t row;    // 0x33
  uint8_t col;    // 0x34
} HIDCursorPos;


#endif
#endif

