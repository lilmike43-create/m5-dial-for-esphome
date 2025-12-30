#pragma once
#include "M5Dial.h"

namespace esphome
{
    namespace shys_m5_dial
    {
        class M5DialRotary {
            protected:
                std::function<void(void)> rotary_left_action;
                std::function<void(void)> rotary_right_action;
                std::function<void(void)> short_button_press_action;
                std::function<void(void)> long_button_press_action;

                int longPressMs = 1500;

                long oldPosition = 0;
                bool longPress = false;

                // Debouncing and smoothing
                unsigned long lastRotaryEvent = 0;
                const unsigned long ROTARY_DEBOUNCE_MS = 50;  // Minimum time between events
                const int ROTARY_THRESHOLD = 2;                // Encoder counts needed to trigger
                long accumulatedDelta = 0;                     // Accumulated encoder changes

            public:
                void on_rotary_right(std::function<void(void)> callback){
                    ESP_LOGD("DEVICE", "register on_rotary_right Callback");
                    this->rotary_right_action = callback;
                }

                void on_rotary_left(std::function<void(void)> callback){
                    ESP_LOGD("DEVICE", "register on_rotary_left Callback");
                    this->rotary_left_action = callback;
                }

                void on_short_button_press(std::function<void(void)> callback){
                    ESP_LOGD("DEVICE", "register on_short_button_press Callback");
                    this->short_button_press_action = callback;
                }

                void on_long_button_press(std::function<void(void)> callback){
                    ESP_LOGD("DEVICE", "register on_long_button_press Callback");
                    this->long_button_press_action = callback;
                }


               /**
                * 
                */
                void setLongPressDuration(int value){
                    longPressMs = value;
                }

               /**
                * Enhanced rotary handling with debouncing and threshold
                */
                void handleRotary(){
                    unsigned long currentMillis = millis();
                    long newPosition = M5Dial.Encoder.read();

                    if (newPosition != this->oldPosition) {
                        // Calculate the change
                        long delta = newPosition - this->oldPosition;
                        accumulatedDelta += delta;

                        // Check if enough time has passed (debouncing)
                        bool timeElapsed = (currentMillis - lastRotaryEvent) >= ROTARY_DEBOUNCE_MS;

                        // Check if accumulated change exceeds threshold
                        bool thresholdMet = abs(accumulatedDelta) >= ROTARY_THRESHOLD;

                        // Only trigger event if both conditions are met
                        if (timeElapsed && thresholdMet) {
                            if(accumulatedDelta > 0){
                                ESP_LOGI("DEVICE", "Rotary right (delta: %ld)", accumulatedDelta);
                                this->rotary_right_action();
                            } else {
                                ESP_LOGI("DEVICE", "Rotary left (delta: %ld)", accumulatedDelta);
                                this->rotary_left_action();
                            }

                            // Reset for next event
                            accumulatedDelta = 0;
                            lastRotaryEvent = currentMillis;
                        }

                        this->oldPosition = newPosition;
                    }
                }

               /**
                * 
                */
                bool handleButtonPress(){
                    bool is_event = false;

                    if (M5Dial.BtnA.wasPressed()) {
                        longPress = false;
                        is_event = true;
                    }

                    if (M5Dial.BtnA.pressedFor(longPressMs)) {
                        M5Dial.Speaker.tone(4000, 200);
                        longPress = true;
                        is_event = true;
                    }

                    if (M5Dial.BtnA.wasReleased()) {
                        if(longPress){
                            this->long_button_press_action();
                            ESP_LOGI("DEVICE", "Long press");
                        } else {
                            this->short_button_press_action();
                            ESP_LOGI("DEVICE", "Short press");
                        }
                        is_event = true;
                    }

                    return is_event;
                }

        };
    }
}
