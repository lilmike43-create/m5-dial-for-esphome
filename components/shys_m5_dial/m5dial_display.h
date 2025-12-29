#pragma once
#include "M5Dial.h"
#include "esphome.h"
#include "default_font_16px.h"
#include "screensaver.h"

#define FF_DEFAULT &default_font_16px


/**
 * M5Dial Display
 *--------------------------
 * Display driver: GC9A01
 * Resolution: 240x240
 * Touch driver: FT3267
 */
namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialDisplay {
            protected:
                uint16_t backgroundColor = YELLOW;

                LovyanGFX* gfx = &M5Dial.Display;

                int timeToScreenOff = 30000;
                unsigned long lastEvent = 0;
                uint16_t lastMode = -1;

                std::string fontName = "default";  //"FreeMono12pt7b";
                float fontFactor = 1;

                int displayRotation = 2;

                Screensaver* screensaver = nullptr;
                bool screensaverRunning = false;

                std::function<void(bool)> display_refresh_action;

            public:
                M5DialDisplay() {
                }

                void init(){
                    M5Dial.Display.setRotation(displayRotation);
                }

                void on_display_refresh(std::function<void(bool)> callback){
                    ESP_LOGD("DEVICE", "register on_swipe Callback");
                    this->display_refresh_action = callback;
                }

                void setTimeToScreenOff(int value){
                    this->timeToScreenOff = value;
                }

                void setRotation(int value){
                    this->displayRotation = value;
                }

                void resetLastEventTimer(){
                    lastEvent = esphome::millis();
                }

                uint16_t getHeight(){
                    return M5Dial.Display.height();
                }
                uint16_t getWidth(){
                    return M5Dial.Display.width();
                }

                LovyanGFX* getGfx() {
                    return gfx;
                }

                void setFontName(std::string name){
                    this->fontName = name;
                }

                void setFontFactor(float factor){
                    this->fontFactor = factor;
                }

                bool isDisplayOn(){
                    return M5Dial.Display.getBrightness() > 0;
                }

                void setBackgroundColor(uint16_t color){
                    this->backgroundColor = backgroundColor;
                }

                uint16_t getBackgroundColor(){
                    return this->backgroundColor;
                }

                void setScreensaver(Screensaver* saver){
                    this->screensaver = saver;
                }

                bool isScreensaverActive(){
                    return this->screensaver != nullptr;
                }
                
                bool isScreensaverRunning(){
                    return screensaverRunning;
                }
                
                void resetScreensaverRunning(){
                    screensaverRunning = false;
                }

                void validateTimeout(){
                    if (esphome::millis() - lastEvent > timeToScreenOff ) {
                        if(this->isScreensaverActive()){
                            bool forceRefresh = !screensaverRunning;
                            screensaver->show(*this, forceRefresh);
                            
                            screensaverRunning = true;
                        } else {
                            if(M5Dial.Display.getBrightness()>0){
                                M5Dial.Display.setBrightness(0);
                                ESP_LOGI("DISPLAY", "Sleep after %d ms", timeToScreenOff);
                            }
                        } 
                    } else {
                        if(screensaverRunning){
                            this->resetScreensaverRunning();
                            this->display_refresh_action(true);
                        }

                        if ( M5Dial.Display.getBrightness()<=0 ) {
                            M5Dial.Display.setBrightness(100);
                            ESP_LOGI("DISPLAY", "Display on");
                        }
                    }
                }


                void showOffline(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextDatum(middle_center);
                    this->setFontByName(this->fontName);

                    gfx->startWrite();                      // Secure SPI bus

                    // Modern gradient background
                    this->clear(ModernUI::BG_DARK);

                    // Draw circular gradient background
                    this->drawCircularGradient(width/2, height/2, 100,
                                              ModernUI::BG_CARD, ModernUI::BG_DARK);

                    // Draw status text with shadow
                    this->setFontsize(2);
                    this->drawTextWithShadow("OFFLINE", width / 2, height / 2, ModernUI::TEXT_DISABLED);

                    // Draw subtle status indicator ring
                    gfx->fillArc(width/2, height/2, 118, 114, 0, 360, ModernUI::TEXT_DISABLED);

                    gfx->endWrite();                      // Release SPI bus
                    this->resetScreensaverRunning();
                }

                void showDisconnected(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextDatum(middle_center);
                    this->setFontByName(this->fontName);

                    gfx->startWrite();                      // Secure SPI bus

                    // Modern gradient background
                    this->clear(ModernUI::BG_DARK);

                    // Draw pulsing gradient circle
                    this->drawCircularGradient(width/2, height/2, 100,
                                              ModernUI::INFO, ModernUI::BG_DARK);

                    // Draw status text with shadow
                    this->setFontsize(1);
                    this->drawTextWithShadow("DISCONNECTED", width / 2, height / 2, ModernUI::TEXT_PRIMARY);

                    // Draw animated status indicator ring (blue gradient)
                    this->drawGradientArc(width/2, height/2, 118, 114, 0, 360,
                                         ModernUI::PRIMARY_BLUE, ModernUI::PRIMARY_TEAL);

                    gfx->endWrite();                      // Release SPI bus
                    this->resetScreensaverRunning();
                }

                void showUnknown(){
                    uint16_t height = this->getHeight();
                    uint16_t width  = this->getWidth();

                    gfx->setTextDatum(middle_center);
                    this->setFontByName(this->fontName);

                    gfx->startWrite();                      // Secure SPI bus

                    // Modern gradient background
                    this->clear(ModernUI::BG_DARK);

                    // Draw warning gradient circle
                    this->drawCircularGradient(width/2, height/2, 100,
                                              ModernUI::WARNING, ModernUI::BG_DARK);

                    // Draw status text with shadow
                    this->setFontsize(2);
                    this->drawTextWithShadow("UNKNOWN", width / 2, height / 2, ModernUI::TEXT_PRIMARY);

                    // Draw warning indicator ring (orange/amber gradient)
                    this->drawGradientArc(width/2, height/2, 118, 114, 0, 360,
                                         ModernUI::ACCENT_AMBER, ModernUI::ACCENT_CORAL);

                    gfx->endWrite();                      // Release SPI bus
                    this->resetScreensaverRunning();
                }

                float getDegByCoord(uint16_t x, uint16_t y){
                    float mx = M5Dial.Display.width()/2;
                    float my = M5Dial.Display.height()/2;

                    float angle = atan2(y - my, x - mx) * 180.0 / M_PI;
                    //angle = 360 - fmod((angle + 360.0 - 90), 360.0);
                    angle = fmod((angle + 360.0 - 90), 360.0);
                    return angle;
                }

                float getRadiusFromCoord(float touchX, float touchY) {
                    float dx = touchX - (getWidth() / 2.0f);
                    float dy = touchY - (getHeight() / 2.0f);
                    float radius = sqrt(dx * dx + dy * dy);

                    return radius;
                }

                coord getColorCoord(float radius, float degree){
                    coord result;
                    result.x = radius * sin(degree*M_PI/180) + (gfx->width()/2);
                    result.y = radius * cos(degree*M_PI/180) + (gfx->height()/2);
                    return result;
                }

                void drawColorCircleLine(float degree, float r1, float r2, uint32_t color) {
                    uint16_t step = 1;
                    coord c1 = getColorCoord(r1, degree);
                    coord c2 = getColorCoord(r2, degree-step);
                    coord c3 = getColorCoord(r2, degree+step);

                    M5Dial.Display.fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);

                    c1 = getColorCoord(r1, degree);
                    c2 = getColorCoord(r1, degree-step-step);
                    c3 = getColorCoord(r2, degree-step);
                    M5Dial.Display.fillTriangle(c1.x, c1.y, c2.x, c2.y, c3.x, c3.y, color);
                }

                void setFontsize(float size) {
                    getGfx()->setTextSize(size * this->fontFactor);
                }

                int getRowHeight(float fontSize){
                    return (int)this->fontFactor * fontSize;
                }

                void setFontByName(const std::string& name) {
                    if (FONT_MAP.find(name) != FONT_MAP.end()) {
                        this->setFontName(name);
                    } else {
                        this->setFontName("default");
                        ESP_LOGE("DISPLAY", "Font '%s' not found, using default font: 'default'", name.c_str());
                    }

                    if(strcmp(name.c_str(), "default")==0){
                        getGfx()->setFont(FF_DEFAULT);
                    } else {
                        getGfx()->setFont(FONT_MAP[this->fontName]);
                    }
                }

                void drawBitmap(const uint8_t* bmp, int size, uint8_t x, uint8_t y, uint8_t width, uint8_t height){
                    M5Dial.Display.drawJpg(bmp, size, x, y, width, height, 0, 0);
                }

                void drawBitmapTransparent(const uint16_t* bmp, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint32_t transparentColor){
                    M5Dial.Display.pushImage(x, y, width, height, bmp, transparentColor);
                }

                void clear(uint16_t bgColor){
                    M5Dial.Display.fillRect(0, 0, getWidth(), getHeight(), bgColor);
                }

                void clear(){
                    this->clear(this->backgroundColor);
                }

                // Modern UI Helper Functions

                /**
                 * Interpolate between two colors
                 */
                uint16_t interpolateColor(uint16_t color1, uint16_t color2, float ratio) {
                    // Extract RGB565 components
                    uint8_t r1 = (color1 >> 11) & 0x1F;
                    uint8_t g1 = (color1 >> 5) & 0x3F;
                    uint8_t b1 = color1 & 0x1F;

                    uint8_t r2 = (color2 >> 11) & 0x1F;
                    uint8_t g2 = (color2 >> 5) & 0x3F;
                    uint8_t b2 = color2 & 0x1F;

                    // Interpolate
                    uint8_t r = r1 + (r2 - r1) * ratio;
                    uint8_t g = g1 + (g2 - g1) * ratio;
                    uint8_t b = b1 + (b2 - b1) * ratio;

                    // Recombine
                    return (r << 11) | (g << 5) | b;
                }

                /**
                 * Draw a gradient arc (progress bar with gradient)
                 */
                void drawGradientArc(int16_t cx, int16_t cy, int16_t r_outer, int16_t r_inner,
                                    float start_angle, float end_angle,
                                    uint16_t color_start, uint16_t color_end) {
                    float angle_range = end_angle - start_angle;
                    int steps = (int)(angle_range / 2); // Draw every 2 degrees for smoothness

                    for(int i = 0; i < steps; i++) {
                        float angle1 = start_angle + (angle_range * i / steps);
                        float angle2 = start_angle + (angle_range * (i + 1) / steps);
                        float ratio = (float)i / steps;

                        uint16_t color = interpolateColor(color_start, color_end, ratio);
                        gfx->fillArc(cx, cy, r_outer, r_inner, angle1, angle2, color);
                    }
                }

                /**
                 * Draw arc with glow effect
                 */
                void drawGlowArc(int16_t cx, int16_t cy, int16_t r_outer, int16_t r_inner,
                                float start_angle, float end_angle, uint16_t color) {
                    // Draw outer glow (wider, dimmer)
                    uint16_t glowColor = interpolateColor(color, ModernUI::BG_DARK, 0.3);
                    gfx->fillArc(cx, cy, r_outer + 2, r_outer, start_angle, end_angle, glowColor);

                    // Draw main arc
                    gfx->fillArc(cx, cy, r_outer, r_inner, start_angle, end_angle, color);

                    // Draw inner highlight
                    uint16_t highlightColor = interpolateColor(color, WHITE, 0.3);
                    gfx->fillArc(cx, cy, r_inner + 2, r_inner, start_angle, end_angle, highlightColor);
                }

                /**
                 * Draw text with shadow for better readability
                 */
                void drawTextWithShadow(const char* text, int16_t x, int16_t y, uint16_t color) {
                    // Draw shadow (offset by 2px)
                    gfx->setTextColor(ModernUI::SHADOW);
                    gfx->drawString(text, x + 2, y + 2);

                    // Draw main text
                    gfx->setTextColor(color);
                    gfx->drawString(text, x, y);
                }

                /**
                 * Draw a circular gradient background
                 */
                void drawCircularGradient(int16_t cx, int16_t cy, int16_t max_radius,
                                         uint16_t center_color, uint16_t edge_color) {
                    for(int r = max_radius; r > 0; r -= 2) {
                        float ratio = (float)(max_radius - r) / max_radius;
                        uint16_t color = interpolateColor(center_color, edge_color, ratio);
                        gfx->drawCircle(cx, cy, r, color);
                    }
                }

                /**
                 * Draw modern card-style background
                 */
                void drawCard(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius = 12) {
                    // Shadow layer
                    gfx->fillRoundRect(x + 3, y + 3, w, h, radius, ModernUI::SHADOW);

                    // Card background
                    gfx->fillRoundRect(x, y, w, h, radius, ModernUI::BG_CARD);
                }

                /**
                 * Get gradient color for progress (3-color gradient)
                 */
                uint16_t getProgressGradientColor(float progress) {
                    // 0-50%: Blue to Purple
                    // 50-100%: Purple to Pink
                    if(progress <= 0.5) {
                        float ratio = progress * 2.0;
                        return interpolateColor(ModernUI::PROGRESS_START, ModernUI::PROGRESS_MID, ratio);
                    } else {
                        float ratio = (progress - 0.5) * 2.0;
                        return interpolateColor(ModernUI::PROGRESS_MID, ModernUI::PROGRESS_END, ratio);
                    }
                }
        };
    }
}