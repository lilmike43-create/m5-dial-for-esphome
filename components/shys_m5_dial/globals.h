#pragma once
#include <map>
#include "M5Dial.h"

#ifndef GLOBALS_H 
#define GLOBALS_H
    static const char *TOUCH_STATE_NONE = "none";
    static const char *TOUCH_STATE_TOUCH = "touch";
    static const char *TOUCH_STATE_TOUCH_END = "touch_end";
    static const char *TOUCH_STATE_TOUCH_BEGIN = "touch_begin";

    static const char *TOUCH_STATE_HOLD = "hold";
    static const char *TOUCH_STATE_HOLD_END = "hold_end";
    static const char *TOUCH_STATE_HOLD_BEGIN = "hold_begin";

    static const char *TOUCH_STATE_FLICK = "flick";
    static const char *TOUCH_STATE_FLICK_END = "flick_end";
    static const char *TOUCH_STATE_FLICK_BEGIN = "flick_begin";

    static const char *TOUCH_STATE_DRAG = "drag";
    static const char *TOUCH_STATE_DRAG_END = "drag_end";
    static const char *TOUCH_STATE_DRAG_BEGIN = "drag_begin";

    static const char *TOUCH_STATE_TMP = "___";

    static const char *TOUCH_STATE_NAME[16] = {
        TOUCH_STATE_NONE, TOUCH_STATE_TOUCH, TOUCH_STATE_TOUCH_END, TOUCH_STATE_TOUCH_BEGIN,
        TOUCH_STATE_TMP, TOUCH_STATE_HOLD, TOUCH_STATE_HOLD_END, TOUCH_STATE_HOLD_BEGIN,
        TOUCH_STATE_TMP, TOUCH_STATE_FLICK, TOUCH_STATE_FLICK_END, TOUCH_STATE_FLICK_BEGIN,
        TOUCH_STATE_TMP, TOUCH_STATE_DRAG, TOUCH_STATE_DRAG_END, TOUCH_STATE_DRAG_BEGIN };

    static const char *TOUCH_SWIPE_NONE = "none";
    static const char *TOUCH_SWIPE_UP = "up";
    static const char *TOUCH_SWIPE_DOWN = "down";
    static const char *TOUCH_SWIPE_LEFT = "left";
    static const char *TOUCH_SWIPE_RIGHT = "right";

    static const char *ROTARY_LEFT = "left";
    static const char *ROTARY_RIGHT = "right";

    static const char *BUTTON_SHORT = "short";
    static const char *BUTTON_LONG = "long";    

    static const char *FAN_DIRECTION_FORWARD = "forward";
    static const char *FAN_DIRECTION_REVERSE = "reverse";
#endif


#ifndef GLOBAL_FONTS_H 
#define GLOBAL_FONTS_H
    static std::map<std::string, const lgfx::GFXfont*> FONT_MAP = {
        {"TomThumb", &fonts::TomThumb},
        {"FreeMono9pt7b", &fonts::FreeMono9pt7b},
        {"FreeMono12pt7b", &fonts::FreeMono12pt7b},
        {"FreeMono18pt7b", &fonts::FreeMono18pt7b},
        {"FreeMono24pt7b", &fonts::FreeMono24pt7b},
        {"FreeMonoBold9pt7b", &fonts::FreeMonoBold9pt7b},
        {"FreeMonoBold12pt7b", &fonts::FreeMonoBold12pt7b},
        {"FreeMonoBold18pt7b", &fonts::FreeMonoBold18pt7b},
        {"FreeMonoBold24pt7b", &fonts::FreeMonoBold24pt7b},
        {"FreeMonoOblique9pt7b", &fonts::FreeMonoOblique9pt7b},
        {"FreeMonoOblique12pt7b", &fonts::FreeMonoOblique12pt7b},
        {"FreeMonoOblique18pt7b", &fonts::FreeMonoOblique18pt7b},
        {"FreeMonoOblique24pt7b", &fonts::FreeMonoOblique24pt7b},
        {"FreeMonoBoldOblique9pt7b", &fonts::FreeMonoBoldOblique9pt7b},
        {"FreeMonoBoldOblique12pt7b", &fonts::FreeMonoBoldOblique12pt7b},
        {"FreeMonoBoldOblique18pt7b", &fonts::FreeMonoBoldOblique18pt7b},
        {"FreeMonoBoldOblique24pt7b", &fonts::FreeMonoBoldOblique24pt7b},
        {"FreeSans9pt7b", &fonts::FreeSans9pt7b},
        {"FreeSans12pt7b", &fonts::FreeSans12pt7b},
        {"FreeSans18pt7b", &fonts::FreeSans18pt7b},
        {"FreeSans24pt7b", &fonts::FreeSans24pt7b},
        {"FreeSansBold9pt7b", &fonts::FreeSansBold9pt7b},
        {"FreeSansBold12pt7b", &fonts::FreeSansBold12pt7b},
        {"FreeSansBold18pt7b", &fonts::FreeSansBold18pt7b},
        {"FreeSansBold24pt7b", &fonts::FreeSansBold24pt7b},
        {"FreeSansOblique9pt7b", &fonts::FreeSansOblique9pt7b},
        {"FreeSansOblique12pt7b", &fonts::FreeSansOblique12pt7b},
        {"FreeSansOblique18pt7b", &fonts::FreeSansOblique18pt7b},
        {"FreeSansOblique24pt7b", &fonts::FreeSansOblique24pt7b},
        {"FreeSansBoldOblique9pt7b", &fonts::FreeSansBoldOblique9pt7b},
        {"FreeSansBoldOblique12pt7b", &fonts::FreeSansBoldOblique12pt7b},
        {"FreeSansBoldOblique18pt7b", &fonts::FreeSansBoldOblique18pt7b},
        {"FreeSansBoldOblique24pt7b", &fonts::FreeSansBoldOblique24pt7b},
        {"FreeSerif9pt7b", &fonts::FreeSerif9pt7b},
        {"FreeSerif12pt7b", &fonts::FreeSerif12pt7b},
        {"FreeSerif18pt7b", &fonts::FreeSerif18pt7b},
        {"FreeSerif24pt7b", &fonts::FreeSerif24pt7b},
        {"FreeSerifItalic9pt7b", &fonts::FreeSerifItalic9pt7b},
        {"FreeSerifItalic12pt7b", &fonts::FreeSerifItalic12pt7b},
        {"FreeSerifItalic18pt7b", &fonts::FreeSerifItalic18pt7b},
        {"FreeSerifItalic24pt7b", &fonts::FreeSerifItalic24pt7b},
        {"FreeSerifBold9pt7b", &fonts::FreeSerifBold9pt7b},
        {"FreeSerifBold12pt7b", &fonts::FreeSerifBold12pt7b},
        {"FreeSerifBold18pt7b", &fonts::FreeSerifBold18pt7b},
        {"FreeSerifBold24pt7b", &fonts::FreeSerifBold24pt7b},
        {"FreeSerifBoldItalic9pt7b", &fonts::FreeSerifBoldItalic9pt7b},
        {"FreeSerifBoldItalic12pt7b", &fonts::FreeSerifBoldItalic12pt7b},
        {"FreeSerifBoldItalic18pt7b", &fonts::FreeSerifBoldItalic18pt7b},
        {"FreeSerifBoldItalic24pt7b", &fonts::FreeSerifBoldItalic24pt7b},
        {"Orbitron_Light_24", &fonts::Orbitron_Light_24},
        {"Orbitron_Light_32", &fonts::Orbitron_Light_32},
        {"Roboto_Thin_24", &fonts::Roboto_Thin_24},
        {"Satisfy_24", &fonts::Satisfy_24},
        {"Yellowtail_32", &fonts::Yellowtail_32},
        {"DejaVu9", &fonts::DejaVu9},
        {"DejaVu12", &fonts::DejaVu12},
        {"DejaVu18", &fonts::DejaVu18},
        {"DejaVu24", &fonts::DejaVu24},
        {"DejaVu40", &fonts::DejaVu40},
        {"DejaVu56", &fonts::DejaVu56},
        {"DejaVu72", &fonts::DejaVu72},
    };
#endif


#ifndef M_PI
    #define M_PI 3.1415926535
#endif

typedef struct coord {
    float x, y;
} COORD;

#ifndef MODERN_UI_COLORS_H
#define MODERN_UI_COLORS_H
    // Modern Color Palette - Sophisticated and elegant
    namespace ModernUI {
        // Primary brand colors (vibrant gradients)
        static const uint16_t PRIMARY_BLUE = 0x1E9F;      // #1E88E5 - Bright blue
        static const uint16_t PRIMARY_PURPLE = 0x8019;    // #8E24AA - Royal purple
        static const uint16_t PRIMARY_TEAL = 0x0659;      // #00BCD4 - Cyan teal
        static const uint16_t PRIMARY_PINK = 0xF81F;      // #FF00FF - Magenta pink

        // Accent colors (modern and vibrant)
        static const uint16_t ACCENT_CORAL = 0xFB20;      // #FF6B40 - Coral
        static const uint16_t ACCENT_LIME = 0x87F0;       // #8BC34A - Lime green
        static const uint16_t ACCENT_AMBER = 0xFD20;      // #FFA000 - Amber
        static const uint16_t ACCENT_ROSE = 0xF99E;       // #F48FB1 - Rose

        // Background colors (dark theme)
        static const uint16_t BG_DARK = 0x1082;           // #121212 - Almost black
        static const uint16_t BG_DARKER = 0x0000;         // #000000 - Pure black
        static const uint16_t BG_CARD = 0x2104;           // #212121 - Card background
        static const uint16_t BG_ELEVATED = 0x3186;       // #303030 - Elevated surface

        // Text colors (high contrast)
        static const uint16_t TEXT_PRIMARY = 0xFFFF;      // #FFFFFF - White
        static const uint16_t TEXT_SECONDARY = 0xBDF7;    // #B0B0B0 - Light gray
        static const uint16_t TEXT_DISABLED = 0x7BEF;     // #707070 - Disabled gray
        static const uint16_t TEXT_ON_COLOR = 0xFFFF;     // #FFFFFF - Text on colored bg

        // Semantic colors (status indicators)
        static const uint16_t SUCCESS = 0x07E0;           // #00FF00 - Success green
        static const uint16_t WARNING = 0xFD20;           // #FFA000 - Warning amber
        static const uint16_t ERROR = 0xF800;             // #FF0000 - Error red
        static const uint16_t INFO = 0x1E9F;              // #1E88E5 - Info blue

        // Progress bar colors (gradient-ready)
        static const uint16_t PROGRESS_START = 0x1E9F;    // #1E88E5 - Blue start
        static const uint16_t PROGRESS_MID = 0x8019;      // #8E24AA - Purple mid
        static const uint16_t PROGRESS_END = 0xF81F;      // #FF00FF - Magenta end
        static const uint16_t PROGRESS_BG = 0x2965;       // #252525 - Progress background

        // Special effects
        static const uint16_t GLOW = 0x051F;              // Subtle glow effect
        static const uint16_t SHADOW = 0x0841;            // Shadow overlay

        // UI spacing constants
        static const int SPACING_XS = 4;
        static const int SPACING_SM = 8;
        static const int SPACING_MD = 12;
        static const int SPACING_LG = 16;
        static const int SPACING_XL = 24;

        // Arc/Ring dimensions
        static const int ARC_OUTER_RADIUS = 118;
        static const int ARC_INNER_RADIUS = 102;
        static const int ARC_THICKNESS = 16;
        static const int ARC_START_ANGLE = 150;
        static const int ARC_END_ANGLE = 390;

        // Animation constants
        static const int TRANSITION_SPEED = 50;  // ms for smooth transitions
        static const float EASE_FACTOR = 0.2;    // Easing interpolation
    }
#endif