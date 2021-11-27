#include <HIDAuxiliaryDisplay.h>
#include <LiquidCrystal.h>

#define HID_AUXD_DISPLAY_PARAMS      0x01 // Display parameters report
#define HID_AUXD_CURSOR_POS          0x02 // Cursor position report
#define HID_AUXD_CHAR_REPORT         0x03 // Character report
#define HID_AUXD_FONT_REPORT         0x04 // Font report
#define HID_AUXD_CTRL_REPORT         0x05 // Control report
#define HID_AUXD_CURSOR_FLAGS        0x06 // Cursor control flags


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

const char STRING_MANUFACTURER[] PROGMEM = "HID Ltd";
const char STRING_PRODUCT[] PROGMEM = "HIDLCD 1602A";
const char STRING_SERIAL[] PROGMEM = "LCD1602A"; 

const byte DISPLAY_WIDTH   = 16;
const byte DISPLAY_HEIGHT  = 2;
const byte CHAR_WIDTH      = 5;
const byte CHAR_HEIGHT     = 8;

const byte PRINTBUFF_SIZE  = DISPLAY_WIDTH + 1;

static const uint8_t _hidReportDescriptor[] PROGMEM = {

    0x05, 0x14,                    // USAGE_PAGE (Alphnumeric Display)
    0x09, 0x01,                    // USAGE (Alphanumeric Display)
    0xa1, 0x02,                    // COLLECTION (Logical)
    0x09, 0x20,                    //   USAGE (Display Attributes Report)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x09, 0x35,                    //     USAGE (Rows)
    0x09, 0x36,                    //     USAGE (Columns)
    0x09, 0x3d,                    //     USAGE (Character Width)
    0x09, 0x3e,                    //     USAGE (Character Height)
    0x85, HID_AUXD_DISPLAY_PARAMS, //     REPORT_ID (1)
    0x75, 0x08,                    //     REPORT_SIZE (8) - changed from 5 to simplify parsing
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x09, 0x21,                    //     USAGE (ASCII Character Set)
    0x09, 0x22,                    //     USAGE (Data Read Back)
    0x09, 0x29,                    //     USAGE (Vertical Scroll)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x95, 0x05,                    //     REPORT_COUNT (5) - byte alignment
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x09, 0x24,                    //   USAGE (Display Control Report)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x09, 0x25,                    //     USAGE (Clear Display)
    0x09, 0x26,                    //     USAGE (Display Enable )
    0x09, 0x28,                    //     USAGE (Screen Saver Enable)
    0x09, 0x29,                    //     USAGE (Vertical Scroll)
    0x09, 0x2A,                    //     USAGE (Horizontal Scroll)
    0x85, HID_AUXD_CTRL_REPORT,     //    REPORT_ID (HID_AUXD_CTRL_REPORT)  
    0x75, 0x08,                    //     REPORT_SIZE (1)
    0x95, 0x05,                    //     REPORT_COUNT (5)    
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
    0x95, 0x03,                    //     REPORT_COUNT (3)    
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)
//    0x09, 0x27,                    //     USAGE (Screen Saver Delay)    
//    0x75, 0x10,                    //     REPORT_SIZE (16)
//    0x95, 0x01,                    //     REPORT_COUNT (1)   
//    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)     
    0xc0,                          //   END_COLLECTION
    0x09, 0x32,                    //   USAGE (Cursor Position Report)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x09, 0x33,                    //     USAGE (Row)
    0x09, 0x34,                    //     USAGE (Column)    
    0x85, HID_AUXD_CURSOR_POS,     //     REPORT_ID (HID_AUXD_CURSOR_POS)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0xb1, 0x22,                    //     FEATURE (Data,Var,Abs,NPrf)
    0x09, 0x37,                    //     USAGE (Cursor Pixel Positioning)  
    0x09, 0x38,                    //     USAGE (Cursor Mode)   
    0x09, 0x39,                    //     USAGE (Cursor Enable)   
    0x09, 0x3A,                    //     USAGE (Cursor Blink)       
    0x85, HID_AUXD_CURSOR_FLAGS,   //     REPORT_ID (HID_AUXD_CURSOR_FLAGS)      
    0x75, 0x01,                    //     REPORT_SIZE (1)    
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)   
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0xb1, 0x03,                    //     FEATURE (Cnst,Var,Abs)       
    0xc0,                          //   END_COLLECTION
    0x09, 0x2b,                    //   USAGE (Character Report) 
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x85, HID_AUXD_CHAR_REPORT,    //     REPORT_ID (3)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, DISPLAY_WIDTH + 1,       //     REPORT_COUNT (4)
    0x09, 0x2c,                    //     USAGE (Display Data)
    0xb2, 0x02, 0x01,              //     FEATURE (Data,Var,Abs,Buf)
    0xc0,                          //   END_COLLECTION
    0x09, 0x3b,                    //   USAGE (Font Report)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x7e,                    //     LOGICAL_MAXIMUM (126)
    0x85, HID_AUXD_FONT_REPORT,    //     REPORT_ID (4)    
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x09, 0x2c,                    //     USAGE (Display Data)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x95, 0x05,                    //     REPORT_COUNT (5)
    0x09, 0x3c,                    //     USAGE (Font Data)
    0x92, 0x02, 0x01,              //     OUTPUT (Data,Var,Abs,Buf)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION                       

};

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

// here we set physical params of our display
const HIDDisplayParams DisplayParams = 
    {.rows=DISPLAY_HEIGHT, 
     .cols=DISPLAY_WIDTH, 
     .chrw=CHAR_WIDTH, 
     .chrh=CHAR_HEIGHT,
     .flags=HID_AUXD_ASCII_CHARSET };

uint8_t dc_flags =  HID_AUXD_ENABLE;

HIDCursorPos CursorPos = {0,0};

uint8_t cc_flags =   HID_ADCC_INCREMENT;   // 0x37|0x38|0x39|0x3A|4 unused

char printbuff[PRINTBUFF_SIZE];

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 14, en = 16, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
    
    Serial.begin(9600);

    HID().begin(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    
    // Serial No is set in a special way as it forms Arduino port name
    HID().setSerial(STRING_SERIAL); 

    // Used for debugging purposes. 
    HID().setOutput(Serial);
    
    HID().SetFeature(HID_AUXD_DISPLAY_PARAMS, &DisplayParams, sizeof(HIDDisplayParams));
    HID().SetFeature(HID_AUXD_CURSOR_POS, &CursorPos, sizeof(CursorPos));
    HID().SetFeature(HID_AUXD_CTRL_REPORT, &dc_flags, sizeof(dc_flags));
    HID().SetFeature(HID_AUXD_CURSOR_FLAGS, &cc_flags, sizeof(cc_flags));    
    HID().SetFeature(HID_AUXD_CHAR_REPORT, printbuff, PRINTBUFF_SIZE);

    lcd.begin(DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

void loop() {
    if(HID().IsHostUpdated(HID_AUXD_CTRL_REPORT)) {
        if(!(dc_flags & HID_AUXD_ENABLE))
            lcd.noDisplay();
        else {
            lcd.display();
            if(dc_flags & HID_AUXD_CLEAR) {
                lcd.clear();
                dc_flags &= ~(HID_AUXD_CLEAR); // reset clear flag after command is executed
            }
            if(dc_flags & HID_AUXD_HSCROLL) 
                lcd.autoscroll();
            else
                lcd.noAutoscroll();
        }    
    }
    if(dc_flags & HID_AUXD_ENABLE) {
        if(HID().IsHostUpdated(HID_AUXD_CURSOR_POS)) {
            lcd.setCursor(CursorPos.col, CursorPos.row);           
        }
        if(HID().IsHostUpdated(HID_AUXD_CURSOR_FLAGS)) {
            if(cc_flags & HID_ADCC_INCREMENT) 
                lcd.leftToRight();
            else
                lcd.rightToLeft();
            if(cc_flags & HID_ADCC_ENABLE) 
                lcd.cursor();
            else
                lcd.noCursor();
            if(cc_flags & HID_ADCC_BLINK) 
                lcd.blink();
            else
                lcd.noBlink(); 
        }
        if(HID().IsHostUpdated(HID_AUXD_CHAR_REPORT)) {
            lcd.print(printbuff);
        }
    }
}
