#pragma once
#include "M5Dial.h"
#include "m5dial_display.h"
#include "m5dial_eeprom.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        // Settings menu item types
        enum SettingType {
            SETTING_CATEGORY,    // Category header
            SETTING_VALUE,       // Adjustable value
            SETTING_TOGGLE,      // On/off toggle
            SETTING_SELECT,      // Multiple choice
            SETTING_ACTION       // Button action
        };

        // Settings menu categories
        enum SettingCategory {
            CAT_MAIN,           // Main settings menu
            CAT_DISPLAY,        // Display settings
            CAT_DEVICES,        // Device management
            CAT_THEMES,         // Theme selection
            CAT_TIME,           // Time settings
            CAT_ADVANCED        // Advanced options
        };

        // Theme definitions
        struct Theme {
            const char* name;
            uint16_t primaryColor;
            uint16_t secondaryColor;
            uint16_t accentColor;
            uint16_t bgDark;
            uint16_t textPrimary;
            uint16_t textSecondary;
        };

        // Predefined themes
        namespace Themes {
            const Theme MODERN_BLUE = {
                "Modern Blue",
                0x1E9F,  // Blue
                0x8019,  // Purple
                0xFBE0,  // Coral
                0x1082,  // Dark bg
                0xFFFF,  // White text
                0xAD55   // Gray text
            };

            const Theme WARM_SUNSET = {
                "Warm Sunset",
                0xFD20,  // Orange
                0xF800,  // Red
                0xFFE0,  // Yellow
                0x2104,  // Dark brown
                0xFFFF,  // White text
                0xC618   // Tan text
            };

            const Theme COOL_MINT = {
                "Cool Mint",
                0x07FF,  // Cyan
                0x07E0,  // Green
                0xAFE5,  // Aqua
                0x0841,  // Dark teal
                0xFFFF,  // White text
                0x9CF3   // Light cyan text
            };

            const Theme DARK_PURPLE = {
                "Dark Purple",
                0x8811,  // Purple
                0xF81F,  // Magenta
                0xA81F,  // Deep pink
                0x2005,  // Very dark purple
                0xFFFF,  // White text
                0xB5F7   // Lavender text
            };

            const Theme* ALL_THEMES[] = {
                &MODERN_BLUE,
                &WARM_SUNSET,
                &COOL_MINT,
                &DARK_PURPLE
            };

            const int THEME_COUNT = 4;
        }

        // Settings menu item structure
        struct SettingsMenuItem {
            const char* label;
            SettingType type;
            SettingCategory category;
            int* valuePtr;      // Pointer to value for SETTING_VALUE
            int minValue;
            int maxValue;
            int step;
            const char* unit;
            void (*action)();   // Function pointer for SETTING_ACTION
        };

        /**
         * Settings menu manager class
         */
        class SettingsMenu {
        protected:
            M5DialEEPROM* eeprom;
            int currentCategory = CAT_MAIN;
            int currentItem = 0;
            bool inEditMode = false;

            // Settings values (stored in EEPROM)
            int screenTimeout = 30000;
            int longPressDuration = 1200;
            int rotaryStepWidth = 10;
            int displayBrightness = 255;
            int displayRotation = 0;
            int currentTheme = 0;
            int fontFactor = 10;  // 1.0 stored as 10
            bool screensaverEnabled = true;

            // Temporary edit value
            int editValue = 0;

        public:
            SettingsMenu(M5DialEEPROM* eeprom) : eeprom(eeprom) {
                loadSettings();
            }

            /**
             * Load settings from EEPROM
             */
            void loadSettings() {
                screenTimeout = eeprom->readInt(100, 30000);
                longPressDuration = eeprom->readInt(104, 1200);
                rotaryStepWidth = eeprom->readInt(108, 10);
                displayBrightness = eeprom->readInt(112, 255);
                displayRotation = eeprom->readInt(116, 0);
                currentTheme = eeprom->readInt(120, 0);
                fontFactor = eeprom->readInt(124, 10);
                screensaverEnabled = eeprom->readInt(128, 1);

                ESP_LOGI("SETTINGS", "Settings loaded from EEPROM");
            }

            /**
             * Save settings to EEPROM
             */
            void saveSettings() {
                eeprom->writeInt(100, screenTimeout);
                eeprom->writeInt(104, longPressDuration);
                eeprom->writeInt(108, rotaryStepWidth);
                eeprom->writeInt(112, displayBrightness);
                eeprom->writeInt(116, displayRotation);
                eeprom->writeInt(120, currentTheme);
                eeprom->writeInt(124, fontFactor);
                eeprom->writeInt(128, screensaverEnabled ? 1 : 0);

                ESP_LOGI("SETTINGS", "Settings saved to EEPROM");
            }

            /**
             * Get current theme
             */
            const Theme* getCurrentTheme() {
                if (currentTheme >= 0 && currentTheme < Themes::THEME_COUNT) {
                    return Themes::ALL_THEMES[currentTheme];
                }
                return &Themes::MODERN_BLUE;
            }

            /**
             * Render main settings menu
             */
            void renderMainMenu(M5DialDisplay& display) {
                LovyanGFX* gfx = display.getGfx();
                const Theme* theme = getCurrentTheme();

                int width = gfx->width();
                int height = gfx->height();

                gfx->startWrite();

                // Modern dark background
                display.clear(theme->bgDark);

                // Title with gradient arc
                display.setFontsize(1.2);
                gfx->setTextColor(theme->textPrimary);
                gfx->setTextDatum(middle_center);
                display.drawTextWithShadow("Settings", width / 2, 30, theme->textPrimary);

                // Menu items with modern styling
                const char* menuItems[] = {
                    "Display",
                    "Devices",
                    "Themes",
                    "Time",
                    "Advanced",
                    "< Exit"
                };

                int itemCount = 6;
                int itemY = 65;
                int itemHeight = 25;

                for (int i = 0; i < itemCount; i++) {
                    bool isSelected = (i == currentItem);

                    if (isSelected) {
                        // Selected item: gradient background
                        gfx->fillRoundRect(20, itemY - 2, width - 40, itemHeight, 8, theme->primaryColor);
                        gfx->drawRoundRect(20, itemY - 2, width - 40, itemHeight, 8, theme->accentColor);
                    }

                    // Item text
                    display.setFontsize(0.95);
                    gfx->setTextColor(isSelected ? 0xFFFF : theme->textSecondary);
                    gfx->setTextDatum(middle_left);
                    gfx->drawString(menuItems[i], 35, itemY + itemHeight / 2);

                    // Arrow indicator for selected
                    if (isSelected) {
                        gfx->fillTriangle(
                            width - 35, itemY + itemHeight / 2 - 5,
                            width - 35, itemY + itemHeight / 2 + 5,
                            width - 28, itemY + itemHeight / 2,
                            theme->accentColor
                        );
                    }

                    itemY += itemHeight + 3;
                }

                // Rotary hint at bottom
                display.setFontsize(0.7);
                gfx->setTextColor(theme->textSecondary);
                gfx->setTextDatum(middle_center);
                gfx->drawString("Rotate: Navigate | Press: Select", width / 2, height - 15);

                gfx->endWrite();
            }

            /**
             * Render display settings submenu
             */
            void renderDisplaySettings(M5DialDisplay& display) {
                LovyanGFX* gfx = display.getGfx();
                const Theme* theme = getCurrentTheme();

                int width = gfx->width();
                int height = gfx->height();

                gfx->startWrite();
                display.clear(theme->bgDark);

                // Title
                display.setFontsize(1.0);
                gfx->setTextColor(theme->textPrimary);
                gfx->setTextDatum(middle_center);
                display.drawTextWithShadow("Display Settings", width / 2, 25, theme->textPrimary);

                // Settings items
                const char* labels[] = {
                    "Timeout (s):",
                    "Brightness:",
                    "Rotation:",
                    "Font Size:",
                    "Screensaver:",
                    "< Back"
                };

                char values[6][20];
                snprintf(values[0], 20, "%d", screenTimeout / 1000);
                snprintf(values[1], 20, "%d", displayBrightness);
                snprintf(values[2], 20, "%d", displayRotation);
                snprintf(values[3], 20, "%.1f", fontFactor / 10.0);
                snprintf(values[4], 20, "%s", screensaverEnabled ? "ON" : "OFF");
                snprintf(values[5], 20, "");

                int itemY = 55;
                int itemHeight = 25;

                for (int i = 0; i < 6; i++) {
                    bool isSelected = (i == currentItem);

                    if (isSelected && inEditMode) {
                        // Editing: highlighted with accent color
                        gfx->fillRoundRect(10, itemY - 2, width - 20, itemHeight, 8, theme->accentColor);
                    } else if (isSelected) {
                        // Selected: normal highlight
                        gfx->fillRoundRect(10, itemY - 2, width - 20, itemHeight, 8, theme->primaryColor);
                    }

                    // Label
                    display.setFontsize(0.85);
                    gfx->setTextColor(isSelected ? 0xFFFF : theme->textSecondary);
                    gfx->setTextDatum(middle_left);
                    gfx->drawString(labels[i], 20, itemY + itemHeight / 2);

                    // Value (if not back button)
                    if (i < 5) {
                        gfx->setTextDatum(middle_right);
                        uint16_t valueColor = (isSelected && inEditMode) ? 0xFFFF : theme->accentColor;
                        gfx->setTextColor(valueColor);
                        gfx->drawString(values[i], width - 20, itemY + itemHeight / 2);
                    }

                    itemY += itemHeight + 3;
                }

                // Instructions
                display.setFontsize(0.7);
                gfx->setTextColor(theme->textSecondary);
                gfx->setTextDatum(middle_center);
                if (inEditMode) {
                    gfx->drawString("Rotate: Adjust | Press: Save", width / 2, height - 15);
                } else {
                    gfx->drawString("Rotate: Navigate | Press: Edit", width / 2, height - 15);
                }

                gfx->endWrite();
            }

            /**
             * Render theme selection menu
             */
            void renderThemeSelection(M5DialDisplay& display) {
                LovyanGFX* gfx = display.getGfx();
                const Theme* theme = getCurrentTheme();

                int width = gfx->width();
                int height = gfx->height();

                gfx->startWrite();
                display.clear(theme->bgDark);

                // Title
                display.setFontsize(1.0);
                gfx->setTextColor(theme->textPrimary);
                gfx->setTextDatum(middle_center);
                display.drawTextWithShadow("Select Theme", width / 2, 25, theme->textPrimary);

                // Theme preview circles
                int startY = 60;
                int itemHeight = 35;

                for (int i = 0; i < Themes::THEME_COUNT; i++) {
                    const Theme* t = Themes::ALL_THEMES[i];
                    bool isSelected = (i == currentItem);
                    int itemY = startY + i * itemHeight;

                    // Selection highlight
                    if (isSelected) {
                        gfx->fillRoundRect(10, itemY - 2, width - 20, itemHeight - 3, 8, theme->primaryColor);
                    }

                    // Theme color preview circles
                    gfx->fillCircle(30, itemY + 13, 10, t->primaryColor);
                    gfx->fillCircle(50, itemY + 13, 10, t->secondaryColor);
                    gfx->fillCircle(70, itemY + 13, 10, t->accentColor);

                    // Theme name
                    display.setFontsize(0.9);
                    gfx->setTextColor(isSelected ? 0xFFFF : theme->textSecondary);
                    gfx->setTextDatum(middle_left);
                    gfx->drawString(t->name, 90, itemY + 13);

                    // Checkmark if current theme
                    if (i == currentTheme) {
                        gfx->setTextColor(theme->accentColor);
                        gfx->setTextDatum(middle_right);
                        gfx->drawString("✓", width - 20, itemY + 13);
                    }
                }

                // Back button
                int backY = startY + Themes::THEME_COUNT * itemHeight;
                bool backSelected = (currentItem == Themes::THEME_COUNT);
                if (backSelected) {
                    gfx->fillRoundRect(10, backY - 2, width - 20, itemHeight - 3, 8, theme->primaryColor);
                }
                display.setFontsize(0.9);
                gfx->setTextColor(backSelected ? 0xFFFF : theme->textSecondary);
                gfx->setTextDatum(middle_left);
                gfx->drawString("< Back", 30, backY + 13);

                // Instructions
                display.setFontsize(0.7);
                gfx->setTextColor(theme->textSecondary);
                gfx->setTextDatum(middle_center);
                gfx->drawString("Rotate: Navigate | Press: Apply", width / 2, height - 15);

                gfx->endWrite();
            }

            /**
             * Handle rotary input in settings
             */
            bool handleRotary(const char* direction) {
                if (inEditMode) {
                    // Adjust value being edited
                    if (currentCategory == CAT_DISPLAY) {
                        int delta = (strcmp(direction, ROTARY_RIGHT) == 0) ? 1 : -1;

                        switch (currentItem) {
                            case 0: // Timeout
                                screenTimeout = constrain(screenTimeout + delta * 5000, 5000, 300000);
                                break;
                            case 1: // Brightness
                                displayBrightness = constrain(displayBrightness + delta * 10, 10, 255);
                                M5Dial.Display.setBrightness(displayBrightness);
                                break;
                            case 2: // Rotation
                                displayRotation = constrain(displayRotation + delta, 0, 7);
                                break;
                            case 3: // Font size
                                fontFactor = constrain(fontFactor + delta, 5, 30);
                                break;
                            case 4: // Screensaver
                                screensaverEnabled = !screensaverEnabled;
                                break;
                        }
                    }
                } else {
                    // Navigate menu
                    int maxItems = 0;
                    if (currentCategory == CAT_MAIN) {
                        maxItems = 6;
                    } else if (currentCategory == CAT_DISPLAY) {
                        maxItems = 6;
                    } else if (currentCategory == CAT_THEMES) {
                        maxItems = Themes::THEME_COUNT + 1;
                    }

                    if (strcmp(direction, ROTARY_RIGHT) == 0) {
                        currentItem = (currentItem + 1) % maxItems;
                    } else {
                        currentItem = (currentItem - 1 + maxItems) % maxItems;
                    }
                }

                return true;
            }

            /**
             * Handle button press in settings
             */
            bool handleButton() {
                if (currentCategory == CAT_MAIN) {
                    // Main menu selection
                    switch (currentItem) {
                        case 0: // Display
                            currentCategory = CAT_DISPLAY;
                            currentItem = 0;
                            break;
                        case 1: // Devices
                            currentCategory = CAT_DEVICES;
                            currentItem = 0;
                            break;
                        case 2: // Themes
                            currentCategory = CAT_THEMES;
                            currentItem = 0;
                            break;
                        case 3: // Time
                            currentCategory = CAT_TIME;
                            currentItem = 0;
                            break;
                        case 4: // Advanced
                            currentCategory = CAT_ADVANCED;
                            currentItem = 0;
                            break;
                        case 5: // Exit
                            return false; // Exit settings
                    }
                } else if (currentCategory == CAT_DISPLAY) {
                    if (currentItem == 5) {
                        // Back to main
                        saveSettings();
                        currentCategory = CAT_MAIN;
                        currentItem = 0;
                    } else {
                        // Toggle edit mode
                        inEditMode = !inEditMode;
                        if (!inEditMode) {
                            saveSettings();
                        }
                    }
                } else if (currentCategory == CAT_THEMES) {
                    if (currentItem == Themes::THEME_COUNT) {
                        // Back to main
                        currentCategory = CAT_MAIN;
                        currentItem = 0;
                    } else {
                        // Apply theme
                        currentTheme = currentItem;
                        saveSettings();
                        M5Dial.Speaker.tone(6000, 50);
                    }
                }

                return true;
            }

            /**
             * Render current settings view
             */
            void render(M5DialDisplay& display) {
                switch (currentCategory) {
                    case CAT_MAIN:
                        renderMainMenu(display);
                        break;
                    case CAT_DISPLAY:
                        renderDisplaySettings(display);
                        break;
                    case CAT_THEMES:
                        renderThemeSelection(display);
                        break;
                    default:
                        renderMainMenu(display);
                        break;
                }
            }

            // Getters for settings values
            int getScreenTimeout() { return screenTimeout; }
            int getLongPressDuration() { return longPressDuration; }
            int getRotaryStepWidth() { return rotaryStepWidth; }
            int getDisplayBrightness() { return displayBrightness; }
            int getDisplayRotation() { return displayRotation; }
            float getFontFactor() { return fontFactor / 10.0; }
            bool isScreensaverEnabled() { return screensaverEnabled; }
        };
    }
}
