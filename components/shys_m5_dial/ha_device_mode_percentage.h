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

                // Performance optimization
                int lastDrawnValue = -1;
                unsigned long lastValueChange = 0;
                const unsigned long SETTLE_DELAY = 500;  // Wait 500ms after change before drawing gradients


                void showPercentageMenu(M5DialDisplay& display, bool fullRedraw = true){
                    LovyanGFX* gfx = display.getGfx();

                    uint16_t height = gfx->height();
                    uint16_t width  = gfx->width();

                    gfx->setTextDatum(middle_center);

                    // Track value changes for smart rendering
                    int currentValue = getValue();
                    bool valueChanged = (currentValue != lastDrawnValue);
                    if(valueChanged) {
                        lastValueChange = esphome::millis();
                        lastDrawnValue = currentValue;
                    }

                    // Determine if we should use fast or beautiful rendering
                    bool isSettled = (esphome::millis() - lastValueChange) > SETTLE_DELAY;
                    bool needsArcRedraw = fullRedraw || valueChanged;

                    gfx->startWrite();                      // Secure SPI bus

                    // Calculate progress for arc
                    float progress = (float)(currentValue - this->getMinValue()) / (this->getMaxValue() - this->getMinValue());
                    float valOnArc = (getMaxValue()==0?240:(progress * 240)) + 150;

                    if(fullRedraw) {
                        // Full redraw - draw everything
                        display.clear(ModernUI::BG_DARK);

                        // Draw center circle with gradient (only on full redraw)
                        display.drawCircularGradient(width/2, height/2, 95,
                                                    ModernUI::BG_ELEVATED, ModernUI::BG_DARK);
                        gfx->fillCircle(width/2, height/2, 95, ModernUI::BG_ELEVATED);

                        // Icon with subtle shadow (only on full redraw)
                        if(this->icon != nullptr){
                            display.drawBitmapTransparent(this->icon, width/2-33, height/2-28, 70, 70, 0xFFFF);
                            display.drawBitmapTransparent(this->icon, width/2-35, height/2-30, 70, 70, 0xFFFF);
                        }

                        // Label with secondary text color (only on full redraw)
                        display.setFontsize(1.1);
                        gfx->setTextColor(ModernUI::TEXT_SECONDARY);
                        gfx->drawString(this->label.c_str(), width / 2, height / 2 - 42);
                    }
                    // No clearing during refresh - just overdraw text

                    // Draw arc - only if needed and optimized based on state
                    if(this->isBarActive() && needsArcRedraw){
                        // Background arc (inactive portion)
                        gfx->fillArc(width / 2, height / 2,
                                    ModernUI::ARC_OUTER_RADIUS, ModernUI::ARC_INNER_RADIUS,
                                    150, 390, ModernUI::PROGRESS_BG);

                        if(isSettled) {
                            // Beautiful mode: Use gradients when value has settled
                            uint16_t gradientEnd = display.getProgressGradientColor(progress);
                            display.drawGradientArc(width / 2, height / 2,
                                                  ModernUI::ARC_OUTER_RADIUS,
                                                  ModernUI::ARC_INNER_RADIUS,
                                                  150, valOnArc,
                                                  ModernUI::PROGRESS_START, gradientEnd);

                            // Add subtle glow
                            uint16_t glowColor = display.interpolateColor(gradientEnd, WHITE, 0.3);
                            gfx->fillArc(width / 2, height / 2,
                                        ModernUI::ARC_OUTER_RADIUS + 2,
                                        ModernUI::ARC_OUTER_RADIUS,
                                        150, valOnArc, glowColor);
                        } else {
                            // Fast mode: Use solid color during rotation for smooth updates
                            uint16_t fastColor = display.getProgressGradientColor(progress);
                            gfx->fillArc(width / 2, height / 2,
                                        ModernUI::ARC_OUTER_RADIUS,
                                        ModernUI::ARC_INNER_RADIUS,
                                        150, valOnArc, fastColor);
                        }
                    } else if(!this->isBarActive()) {
                        gfx->fillArc(width / 2, height / 2,
                                    ModernUI::ARC_OUTER_RADIUS,
                                    ModernUI::ARC_INNER_RADIUS,
                                    150, 390, ModernUI::PROGRESS_BG);
                    }

                    // Only update text when value changes or full redraw
                    if(fullRedraw || valueChanged) {
                        // Value text with opaque background to prevent ghosting
                        gfx->fillRect(width/2 - 65, height/2 - 88, 130, 36, ModernUI::BG_ELEVATED);
                        display.setFontsize(2.2);
                        String valueText = use_custom_value ? custom_value.c_str() : (String(currentValue) + this->unit.c_str()).c_str();
                        display.drawTextWithShadow(valueText.c_str(), width / 2, height / 2 - 70, ModernUI::TEXT_PRIMARY);

                        // Device name at bottom with opaque background
                        gfx->fillRect(width/2 - 65, height/2 + 76, 130, 18, ModernUI::BG_DARK);
                        display.setFontsize(1.0);
                        uint16_t nameColor = display.getProgressGradientColor(progress);
                        gfx->setTextColor(nameColor);
                        gfx->drawString(this->device.getName().c_str(), width / 2, height / 2 + 85);

                        // Progress indicator dots with clear background
                        int dotY = height / 2 + 105;
                        int dotSpacing = 8;
                        int numDots = 5;
                        int startX = width / 2 - (numDots - 1) * dotSpacing / 2;

                        gfx->fillRect(width/2 - 28, height/2 + 99, 56, 12, ModernUI::BG_DARK);
                        for(int i = 0; i < numDots; i++){
                            float dotProgress = (float)i / (numDots - 1);
                            uint16_t dotColor = (progress >= dotProgress) ?
                                display.getProgressGradientColor(dotProgress) :
                                ModernUI::PROGRESS_BG;
                            gfx->fillCircle(startX + i * dotSpacing, dotY, 2, dotColor);
                        }
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
                    showPercentageMenu(display, init);  // Full redraw only on init
                }

                // Disable automatic transition refresh to eliminate potential flicker source
                // bool isDisplayRefreshNeeded() override {
                //     unsigned long timeSinceChange = esphome::millis() - lastValueChange;
                //     return (timeSinceChange > SETTLE_DELAY && timeSinceChange < SETTLE_DELAY + 200);
                // }
                
                bool onTouch(M5DialDisplay& display, uint16_t x, uint16_t y) override {
                    return this->defaultOnTouch(display, x, y);        
                }

                bool onRotary(M5DialDisplay& display, const char * direction) override {
                    return this->defaultOnRotary(display, direction);
                }
        };
    }
}