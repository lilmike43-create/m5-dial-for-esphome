#pragma once
#include "m5dial_display.h"
#include "ha_device_light.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeLightColor: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                // Performance optimization
                int lastDrawnValue = -1;
                unsigned long lastValueChange = 0;
                const unsigned long SETTLE_DELAY = 200;  // Shorter delay for color changes
                void sendValueToHomeAssistant(int value) override {
                    ESP_LOGI("LIGHT_COLOR", "Sending color value %i to HA for %s", value, this->device.getEntityId().c_str());
                    haApi.turnLightOn(this->device.getEntityId(), -1, value);
                }

                typedef struct rgb {
                    float r, g, b;
                } RGB;

                typedef struct hsl {
                    float h, s, l;
                } HSL;

                uint32_t getColorByDegree(float degree){
                    return hslToRgb(degree/360.0, 1, .5);
                }
                
                uint32_t getComplementaryByDegree(float degree){
                    uint32_t complementary = degree;
                    if(complementary >= 180){
                        complementary = complementary - 180;
                    } else {
                        complementary = complementary + 180;
                    }
                    return getColorByDegree(complementary);
                }

                /*
                * Converts an HUE to r, g or b.
                * returns float in the set [0, 1].
                */
                float hue2rgb(float p, float q, float t) {
                    if (t < 0) 
                        t += 1;
                    if (t > 1) 
                        t -= 1;
                    if (t < 1./6) 
                        return p + (q - p) * 6 * t;
                    if (t < 1./2) 
                        return q;
                    if (t < 2./3)   
                        return p + (q - p) * (2./3 - t) * 6;
                        
                    return p;
                }

                /*
                * Convert HSL to RGB.
                * http://en.wikipedia.org/wiki/HSL_color_space.
                * 
                * h, s, l [float 0 - 1]
                * 
                * returns color888
                */
                uint32_t hslToRgb(float h, float s, float l) {
                    RGB result;
                    
                    if(0 == s) {
                        result.r = result.g = result.b = l; // achromatic
                    }
                    else {
                        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
                        float p = 2 * l - q;
                        result.r = hue2rgb(p, q, h + 1./3) * 255;
                        result.g = hue2rgb(p, q, h) * 255;
                        result.b = hue2rgb(p, q, h - 1./3) * 255;
                    }

                    return M5Dial.Display.color888(result.r, result.g, result.b);
                }

                void refreshColorMenu(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();

                    int currentValue = getValue();

                    // Track value changes for smart rendering
                    bool valueChanged = (currentValue != lastDrawnValue);
                    if(valueChanged) {
                        lastValueChange = esphome::millis();
                        lastDrawnValue = currentValue;
                    }

                    // Skip redraw if value hasn't changed
                    if(!valueChanged) {
                        return;
                    }

                    uint32_t currentColor = getColorByDegree(currentValue);
                    uint32_t complementary_color = getComplementaryByDegree(currentValue);

                    int height = gfx->height();
                    int width  = gfx->width();

                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                    // Secure SPI bus

                    // Clear only the center circle area (faster than full redraw)
                    gfx->fillCircle(width/2, height/2, 90, ModernUI::BG_DARK);

                    // Draw center circle with solid color (skip gradient for speed)
                    gfx->fillCircle(width/2, height/2, 85, currentColor);

                    // Add outer glow ring
                    uint16_t glowColor = display.interpolateColor(currentColor, WHITE, 0.4);
                    gfx->fillArc(width/2, height/2, 90, 85, 0, 360, glowColor);

                    // Value text with shadow and complementary color for contrast
                    display.setFontsize(1.8);
                    display.drawTextWithShadow(String(currentValue).c_str(),
                                              width / 2, height / 2 - 30,
                                              complementary_color);

                    // Degree symbol and label
                    display.setFontsize(0.9);
                    gfx->setTextColor(complementary_color);
                    gfx->drawString("hue", width / 2, height / 2 - 8);

                    // Device name
                    display.setFontsize(1.0);
                    gfx->drawString(this->device.getName().c_str(),
                                   width / 2, height / 2 + 20);

                    // Color mode label
                    display.setFontsize(0.85);
                    uint16_t labelColor = display.interpolateColor(complementary_color,
                                                                   ModernUI::TEXT_SECONDARY, 0.3);
                    gfx->setTextColor(labelColor);
                    gfx->drawString("RGB Color", width / 2, height / 2 + 45);

                    // Draw enhanced selector with glow
                    // Outer glow
                    display.drawColorCircleLine(360-currentValue, 35, 83, glowColor);
                    // Main indicator
                    display.drawColorCircleLine(360-currentValue, 38, 80, complementary_color);
                    // Inner highlight
                    display.drawColorCircleLine(360-currentValue, 40, 77, WHITE);

                    gfx->endWrite();                      // Release SPI bus
                }

                void showColorMenu(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();

                    int currentValue = getValue();

                    int height = gfx->height();
                    int width  = gfx->width();

                    gfx->startWrite();                      // Secure SPI bus

                    // Modern dark background
                    display.clear(ModernUI::BG_DARK);

                    // Draw enhanced color wheel - optimized with fewer iterations
                    // Draw every 2 degrees instead of every degree for speed
                    for (int i=0; i<360; i+=2){
                        // Main color ring
                        display.drawColorCircleLine(360-i, 95.0, 120.0, getColorByDegree(i));
                        display.drawColorCircleLine(360-i-1, 95.0, 120.0, getColorByDegree(i));

                        // Inner shadow ring (darker)
                        uint32_t baseColor = getColorByDegree(i);
                        uint32_t darkerColor = display.interpolateColor(baseColor, ModernUI::BG_DARK, 0.3);
                        display.drawColorCircleLine(360-i, 90.0, 95.0, darkerColor);
                        display.drawColorCircleLine(360-i-1, 90.0, 95.0, darkerColor);
                    }

                    gfx->endWrite();                      // Release SPI bus

                    refreshColorMenu(display);
                }


            public:
                HaDeviceModeLightColor(HaDevice& device) : HaDeviceMode(device){
                    this->maxValue = 360;
                    this->endlessRotaryValue = true;
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Farbwahl-Modus");
                    if(init){
                        showColorMenu(display);
                    } else {
                        refreshColorMenu(display);
                    }
                }

                void registerHAListener() override {
                    std::string attrName = "hs_color";
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>(attrName), 
                                [this](const std::string &state) {
                                
                        if(this->isValueModified()){
                            return;
                        }

                        std::string colorString = "";          
                        std::string::size_type pos = state.find(',');
                        
                        if (pos != std::string::npos) {
                            colorString = state.substr(1, pos-1);
                        }
                        ESP_LOGD("HA_API", "HS_Color value %s for %s", colorString.c_str(), this->device.getEntityId().c_str());

                        auto val = parse_number<float>(colorString.c_str());
                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Color value in %s for %s", colorString.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setReceivedValue(val.value());
                            ESP_LOGI("HA_API", "Got Color value %f for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    });
                }
                
                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    uint16_t degree = display.getDegByCoord(x, y);
                    setValue(degree);
                    ESP_LOGD("TOUCH", "Neuen Farbwert auf %d gesetzt", degree);
                    
                    return true;
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    return this->defaultOnRotary(display, direction);
                }      

                bool onButton(M5DialDisplay& display, const char * clickType) override {
                    if (strcmp(clickType, BUTTON_SHORT)==0){
                        haApi.toggleLight(this->device.getEntityId());
                        return true;
                    } 
                    return false;
                }


        };
    }
}