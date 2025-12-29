#pragma once

namespace esphome
{
    namespace shys_m5_dial
    {
        class HaDeviceModePercentage: public esphome::shys_m5_dial::HaDeviceMode {
            protected:
                std::string label = "Percentage";
                std::string unit = "%";
                unsigned short* icon = nullptr;

                bool barActive = true;
                bool use_custom_value = false;
                std::string custom_value = "";


                void showPercentageMenu(M5DialDisplay& display){
                    LovyanGFX* gfx = display.getGfx();

                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextDatum(middle_center);

                    gfx->startWrite();                      // Secure SPI bus

                    // Modern dark gradient background
                    display.clear(ModernUI::BG_DARK);

                    // Calculate progress for arc
                    float progress = (float)(getValue() - this->getMinValue()) / (this->getMaxValue() - this->getMinValue());
                    float valOnArc = (getMaxValue()==0?240:(progress * 240)) + 150;

                    if(this->isBarActive()){
                        // Background arc (inactive portion) with subtle color
                        gfx->fillArc(width / 2, height / 2,
                                    ModernUI::ARC_OUTER_RADIUS, ModernUI::ARC_INNER_RADIUS,
                                    150, 390, ModernUI::PROGRESS_BG);

                        // Draw modern gradient progress arc
                        uint16_t gradientStart = display.getProgressGradientColor(0);
                        uint16_t gradientEnd = display.getProgressGradientColor(progress);

                        display.drawGradientArc(width / 2, height / 2,
                                              ModernUI::ARC_OUTER_RADIUS,
                                              ModernUI::ARC_INNER_RADIUS,
                                              150, valOnArc,
                                              ModernUI::PROGRESS_START, gradientEnd);

                        // Add subtle glow to active portion
                        uint16_t glowColor = display.interpolateColor(gradientEnd, WHITE, 0.3);
                        gfx->fillArc(width / 2, height / 2,
                                    ModernUI::ARC_OUTER_RADIUS + 2,
                                    ModernUI::ARC_OUTER_RADIUS,
                                    150, valOnArc, glowColor);
                    } else {
                        gfx->fillArc(width / 2, height / 2,
                                    ModernUI::ARC_OUTER_RADIUS,
                                    ModernUI::ARC_INNER_RADIUS,
                                    150, 390, ModernUI::PROGRESS_BG);
                    }

                    // Draw center circle with gradient
                    display.drawCircularGradient(width/2, height/2, 95,
                                                ModernUI::BG_ELEVATED, ModernUI::BG_DARK);
                    gfx->fillCircle(width/2, height/2, 95, ModernUI::BG_ELEVATED);

                    // Icon with subtle shadow
                    if(this->icon != nullptr){
                        // Draw icon shadow
                        display.drawBitmapTransparent(this->icon, width/2-33, height/2-28, 70, 70, 0xFFFF);
                        // Draw icon
                        display.drawBitmapTransparent(this->icon, width/2-35, height/2-30, 70, 70, 0xFFFF);
                    }

                    // Value text with modern typography and shadow
                    display.setFontsize(2.2);
                    String valueText = use_custom_value ? custom_value.c_str() : (String(getValue()) + this->unit.c_str()).c_str();
                    display.drawTextWithShadow(valueText.c_str(), width / 2, height / 2 - 70, ModernUI::TEXT_PRIMARY);

                    // Label with secondary text color
                    display.setFontsize(1.1);
                    gfx->setTextColor(ModernUI::TEXT_SECONDARY);
                    gfx->drawString(this->label.c_str(), width / 2, height / 2 - 42);

                    // Device name at bottom with accent color
                    display.setFontsize(1.0);
                    uint16_t nameColor = display.getProgressGradientColor(progress);
                    gfx->setTextColor(nameColor);
                    gfx->drawString(this->device.getName().c_str(), width / 2, height / 2 + 85);

                    // Add progress indicator dots at bottom
                    int dotY = height / 2 + 105;
                    int dotSpacing = 8;
                    int numDots = 5;
                    int startX = width / 2 - (numDots - 1) * dotSpacing / 2;

                    for(int i = 0; i < numDots; i++){
                        float dotProgress = (float)i / (numDots - 1);
                        uint16_t dotColor = (progress >= dotProgress) ?
                            display.getProgressGradientColor(dotProgress) :
                            ModernUI::PROGRESS_BG;
                        gfx->fillCircle(startX + i * dotSpacing, dotY, 2, dotColor);
                    }

                    gfx->endWrite();                      // Release SPI bus
                }

            public:
                HaDeviceModePercentage(HaDevice& device) : HaDeviceMode(device){}

                ~HaDeviceModePercentage() {
                    delete[] icon;
                }

                void setLabel(const std::string& newLabel){
                    this->label = newLabel;
                }

                void setCustomValue(const std::string& newVal){
                    this->custom_value = newVal;
                }

                void useCustomValue(bool activate){
                    this->use_custom_value = activate;
                }

                void setUnit(const std::string& newUnit){
                    this->unit = newUnit;
                }

                void setIcon(const unsigned short* newIcon, size_t size) {
                    delete[] this->icon;
                    this->icon = new unsigned short[size];
                    std::copy(newIcon, newIcon + size, icon);
                }

                void activateBar(bool activate){
                    this->barActive = activate;
                }

                bool isBarActive(){
                    return this->barActive;
                }

                void refreshDisplay(M5DialDisplay& display, bool init) override {
                    ESP_LOGD("DISPLAY", "refresh Display: Percentage-Modus");
                    showPercentageMenu(display);
                }
                
                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    return this->defaultOnTouch(display, x, y);        
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    return this->defaultOnRotary(display, direction);
                }
        };
    }
}