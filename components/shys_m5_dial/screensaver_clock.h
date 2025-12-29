#pragma once
#include "screensaver.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class ScreensaverClock: public esphome::shys_m5_dial::Screensaver {
            protected:
                esphome::time::RealTimeClock* localTime;

                uint8_t lastHr  = 255;
                uint8_t lastMin = 255;
                uint8_t lastSec = 255;

                void showClock(M5DialDisplay& display){
                    // Modern dark gradient background
                    display.clear(ModernUI::BG_DARKER);

                    int cx = display.getWidth() / 2;
                    int cy = display.getHeight() / 2;

                    // Draw subtle outer ring
                    display.getGfx()->fillArc(cx, cy, 120, 118, 0, 360, ModernUI::BG_CARD);

                    // Draw hour markers with modern style
                    for(int i=1; i<=12; i++){
                        int degree = i*30;
                        bool isQuarterHour = (i % 3 == 0);

                        // Use different colors for quarter hours
                        uint16_t markerColor = isQuarterHour ?
                            ModernUI::PRIMARY_BLUE : ModernUI::TEXT_SECONDARY;

                        // Different sizes for quarter hours vs regular hours
                        int innerRadius = isQuarterHour ? 108 : 112;
                        int markerWidth = isQuarterHour ? 6 : 2;

                        coord c1 = display.getColorCoord(innerRadius, degree);
                        coord c2 = display.getColorCoord(118, degree - markerWidth);
                        coord c3 = display.getColorCoord(118, degree + markerWidth);

                        display.getGfx()->fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, markerColor);
                    }

                    // Draw minute markers (subtle dots)
                    for(int i=0; i<60; i++){
                        if(i % 5 != 0) { // Skip where hour markers are
                            int degree = i * 6;
                            coord c = display.getColorCoord(116, degree);
                            display.getGfx()->fillCircle(c.x, c.y, 1, ModernUI::TEXT_DISABLED);
                        }
                    }

                    // Draw center decorative circle
                    display.getGfx()->fillCircle(cx, cy, 100, ModernUI::BG_DARK);
                    display.getGfx()->fillArc(cx, cy, 102, 100, 0, 360, ModernUI::BG_ELEVATED);
                }

                void refreshClock(M5DialDisplay& display){
                    int x = display.getWidth()/2;
                    int y = display.getHeight()/2;

                    uint8_t hour = this->localTime->now().hour;
                    if(lastHr != hour){
                        display.getGfx()->fillCircle(x, y, 100, ModernUI::BG_DARK);
                        lastHr  = 255;
                        lastMin = 255;
                        lastSec = 255;
                    }

                    this->paintHourHand(display);
                    this->paintMinuteHand(display);
                    this->paintSecondHand(display);

                    // Modern center dot with gradient effect
                    display.getGfx()->fillCircle(x, y, 7, ModernUI::BG_ELEVATED);
                    display.getGfx()->fillCircle(x, y, 5, ModernUI::PRIMARY_BLUE);
                    display.getGfx()->fillCircle(x, y, 2, WHITE);
                }

                void paintHourHand(M5DialDisplay& display){
                    uint8_t hour = this->localTime->now().hour;
                    uint8_t minute = this->localTime->now().minute;

                    if(lastHr != hour || lastMin != minute){
                        int degree = 180 - (lastHr * 360 / 12);
                        degree = degree - (round(lastMin/12) * 360 / 60);
                        paintHand(display, degree, 32, 70, ModernUI::BG_DARK);
                    }

                    int degree = 180 - (hour * 360 / 12);
                    degree = degree - (round(minute/12) * 360 / 60);

                    // Draw shadow for depth
                    paintHand(display, degree, 32, 72, ModernUI::SHADOW);
                    // Draw gradient hour hand (blue to white)
                    paintHand(display, degree, 30, 70, ModernUI::PRIMARY_BLUE);
                    // Add highlight to the hand
                    paintHand(display, degree, 12, 70, WHITE);

                    lastHr = hour;
                }

                void paintMinuteHand(M5DialDisplay& display){
                    uint8_t minute = this->localTime->now().minute;

                    if(lastMin != minute){
                        int degree = 180 - (lastMin * 360 / 60);
                        paintHand(display, degree, 22, 95, ModernUI::BG_DARK);
                    }

                    int degree = 180 - (minute * 360 / 60);

                    // Draw shadow for depth
                    paintHand(display, degree, 22, 97, ModernUI::SHADOW);
                    // Draw gradient minute hand (purple to white)
                    paintHand(display, degree, 20, 95, ModernUI::PRIMARY_PURPLE);
                    // Add highlight to the hand
                    paintHand(display, degree, 8, 95, WHITE);

                    lastMin = minute;
                }

                void paintSecondHand(M5DialDisplay& display){
                    uint8_t second = this->localTime->now().second;

                    if(lastSec != second){
                        int degree = 180 - (lastSec * 360 / 60);
                        paintHand(display, degree, 2, 105, ModernUI::BG_DARK);
                    }

                    int degree = 180 - (second * 360 / 60);

                    // Draw modern second hand with glow
                    paintHand(display, degree, 3, 107, ModernUI::ACCENT_CORAL);
                    paintHand(display, degree, 2, 105, ModernUI::ACCENT_CORAL);
                    paintHand(display, degree, 1, 105, WHITE);

                    lastSec = second;
                }

                void paintHand(M5DialDisplay& display, int degree, int width, int length, uint16_t color){
                        coord c1 = display.getColorCoord(length, degree);
                        coord c2 = display.getColorCoord(4, degree - width);
                        coord c3 = display.getColorCoord(4, degree + width);

                        display.getGfx()->fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);
                }

            public:
                void show(M5DialDisplay& display, bool init) override {
                    if(init){
                        showClock(display);
                    } else {
                        refreshClock(display);
                    }
                }

                void setLocalTime(esphome::time::RealTimeClock* time_comp) {
                    this->localTime = time_comp;
                }

        };

    }
}