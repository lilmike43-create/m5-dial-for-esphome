#pragma once
#include <math.h>

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModeLightTunableWhite: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                typedef struct coord {
                    float x, y;
                } COORD;

                void sendValueToHomeAssistant(int value) override {
                    haApi.turnLightOnWhite(this->device.getEntityId(), value);
                }

                uint32_t colorTemperatureToRGB888(int kelvin){
                    int temp = kelvin / 100;
                    int red, green, blue;

                    if( temp <= 66 ){ 
                        red = 255; 
                        green = temp;
                        green = 99.4708025861 * log(green) - 161.1195681661;
                        
                        if( temp <= 19){
                            blue = 0;
                        } else {
                            blue = temp-10;
                            blue = 138.5177312231 * log(blue) - 305.0447927307;
                        }
                    } else {
                        red = temp - 60;
                        red = 329.698727446 * pow(red, -0.1332047592);
                        
                        green = temp - 60;
                        green = 288.1221695283 * pow(green, -0.0755148492 );

                        blue = 255;
                    }

                    return M5Dial.Display.color888(clamp(red,   0, 255), clamp(green, 0, 255), clamp(blue,  0, 255));
                }

                uint16_t colorTemperatureToRGB(int kelvin){
                    int temp = kelvin / 100;
                    int red, green, blue;

                    if( temp <= 66 ){ 
                        red = 255; 
                        green = temp;
                        green = 99.4708025861 * log(green) - 161.1195681661;
                        
                        if( temp <= 19){
                            blue = 0;
                        } else {
                            blue = temp-10;
                            blue = 138.5177312231 * log(blue) - 305.0447927307;
                        }
                    } else {
                        red = temp - 60;
                        red = 329.698727446 * pow(red, -0.1332047592);
                        
                        green = temp - 60;
                        green = 288.1221695283 * pow(green, -0.0755148492 );

                        blue = 255;
                    }

                    return M5Dial.Display.color565(clamp(red,   0, 255), clamp(green, 0, 255), clamp(blue,  0, 255));
                }


                int clamp(int x, int min, int max ) {
                    if(x<min){ return min; }
                    if(x>max){ return max; }
                    return x;
                }


                void showTunableWhiteMenu(M5DialDisplay& display, uint16_t currentValue){
                    LovyanGFX* gfx = display.getGfx();

                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    uint16_t ypos = getDisplayPositionY(currentValue);

                    gfx->setTextColor(MAROON);
                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                  
                    display.clear(colorTemperatureToRGB(currentValue));

                    gfx->drawLine(0, ypos, width, ypos, RED );

                    display.setFontsize(3);
                    gfx->drawString((String(currentValue) + "K").c_str(),
                                    width / 2,
                                    height / 2 - 30);

                    display.setFontsize(1);
                    gfx->drawString(this->device.getName().c_str(),
                                    width / 2,
                                    height / 2 + 20);
                    gfx->drawString("White",
                                    width / 2,
                                    height / 2 + 50);  

                    gfx->endWrite();                      // Release SPI bus
                }



                void refreshWhiteMenu(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();

                    int currentValue = getValue();
                    uint16_t currentColor = colorTemperatureToRGB(currentValue);

                    int height = gfx->height();
                    int width  = gfx->width();

                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                    // Secure SPI bus

                    // Draw center circle with current temperature color
                    display.drawCircularGradient(width/2, height/2, 85,
                                                currentColor, ModernUI::BG_DARK);
                    gfx->fillCircle(width/2, height/2, 85, currentColor);

                    // Add warm glow effect around the center
                    uint16_t glowColor = display.interpolateColor(currentColor, WHITE, 0.5);
                    gfx->fillArc(width/2, height/2, 90, 85, 0, 360, glowColor);

                    // Temperature value with shadow
                    display.setFontsize(2.0);
                    String tempText = String(currentValue) + "K";
                    display.drawTextWithShadow(tempText.c_str(),
                                              width / 2, height / 2 - 35,
                                              ModernUI::TEXT_PRIMARY);

                    // Temperature label
                    display.setFontsize(0.9);
                    gfx->setTextColor(ModernUI::TEXT_SECONDARY);

                    // Show warm/cool indicator
                    const char* tempLabel = (currentValue < 4000) ? "warm" : "cool";
                    gfx->drawString(tempLabel, width / 2, height / 2 - 10);

                    // Device name
                    display.setFontsize(1.0);
                    gfx->setTextColor(ModernUI::TEXT_PRIMARY);
                    gfx->drawString(this->device.getName().c_str(),
                                   width / 2, height / 2 + 20);

                    // Mode label
                    display.setFontsize(0.85);
                    gfx->setTextColor(ModernUI::TEXT_SECONDARY);
                    gfx->drawString("Color Temp", width / 2, height / 2 + 45);

                    // Draw enhanced position indicator
                    float temp = map(currentValue, this->minValue, this->maxValue, 360, 0);

                    // Outer glow
                    display.drawColorCircleLine(temp, 35, 83, glowColor);
                    // Main indicator (use current temperature color)
                    display.drawColorCircleLine(temp, 38, 80, currentColor);
                    // Inner highlight
                    display.drawColorCircleLine(temp, 40, 77, WHITE);

                    gfx->endWrite();                      // Release SPI bus
                }

                void showWhiteMenu(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();

                    int currentValue = getValue();

                    int height = gfx->height();
                    int width  = gfx->width();

                    gfx->startWrite();                      // Secure SPI bus

                    // Modern dark background
                    display.clear(ModernUI::BG_DARK);

                    // Draw temperature gradient ring (warm to cool)
                    // Outer ring - main temperature gradient
                    for (int i=0; i<360; i++){
                        float tmp = map(i, 0, 360, this->minValue, this->maxValue);
                        display.drawColorCircleLine(360-i, 95.0, 120.0, colorTemperatureToRGB888(tmp));
                    }

                    // Inner shadow ring for depth
                    for (int i=0; i<360; i++){
                        float tmp = map(i, 0, 360, this->minValue, this->maxValue);
                        uint32_t baseColor = colorTemperatureToRGB888(tmp);
                        uint32_t darkerColor = display.interpolateColor(baseColor, ModernUI::BG_DARK, 0.3);
                        display.drawColorCircleLine(360-i, 90.0, 95.0, darkerColor);
                    }

                    // Outer highlight ring for modern look
                    for (int i=0; i<360; i++){
                        float tmp = map(i, 0, 360, this->minValue, this->maxValue);
                        uint32_t baseColor = colorTemperatureToRGB888(tmp);
                        uint32_t lighterColor = display.interpolateColor(baseColor, WHITE, 0.2);
                        display.drawColorCircleLine(360-i, 120.0, 122.0, lighterColor);
                    }

                    gfx->endWrite();                      // Release SPI bus

                    refreshWhiteMenu(display);
                }


            public:
                HaDeviceModeLightTunableWhite(HaDevice& device) : HaDeviceMode(device){
                    this->value = 2000;
                    this->minValue = 2000;
                    this->maxValue = 6500;
                    this->endlessRotaryValue = false;
                }


                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Farbwahl-Modus");
                    if(init){
                        showWhiteMenu(display);
                    } else {
                        refreshWhiteMenu(display);
                    }
                }


                void registerHAListener() override {
                    api::global_api_server->subscribe_home_assistant_state(
                                this->device.getEntityId().c_str(),
                                optional<std::string>("color_temp_kelvin"), 
                                [this](const std::string &state) {

                        if(this->isValueModified()){
                            return;
                        }

                        auto val = parse_number<int>(state);
                        if (!val.has_value()) {
                            this->setReceivedValue(0);
                            ESP_LOGD("HA_API", "No Kelvin value in %s for %s", state.c_str(), this->device.getEntityId().c_str());
                        } else {
                            this->setReceivedValue(round((float)val.value()));
                            ESP_LOGI("HA_API", "Got Kelvin value %i for %s", val.value(), this->device.getEntityId().c_str());
                        }
                    });
                }
                
                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    uint16_t degree = display.getDegByCoord(x, y);
                    float tmp = map(degree, 0, 360, this->minValue, this->maxValue);
                    setValue(tmp);
                    ESP_LOGD("TOUCH", "Neuen Weiß-Wert auf %d gesetzt", tmp);
                    
                    return true;                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    return defaultOnRotary(display, direction);
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