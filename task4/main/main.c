#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define BTN_PIN GPIO_NUM_8


typedef enum {
    STATE_IDLE,
    STATE_PRESSED,
    STATE_HELD,
    STATE_RELEASE
} button_state_t;


static uint32_t get_millis(void) {
    return (uint32_t)(esp_timer_get_time() / 1000);
}

void app_main(void) {

    gpio_config_t btn_conf = {
        .pin_bit_mask = (1ULL << BTN_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE 
    };
    gpio_config(&btn_conf);

    button_state_t state = STATE_IDLE;
    uint32_t state_timer = 0;
    uint32_t press_count = 0;



    while (true) {
        bool btn_high = (gpio_get_level(BTN_PIN) == 1);
        uint32_t now = get_millis();

        switch (state) {
            case STATE_IDLE:
                if (btn_high) {
                    state_timer = now;
                    state = STATE_PRESSED;
                }
                break;

            case STATE_PRESSED:
                if (!btn_high) {
                    state = STATE_IDLE;
                } else if (now - state_timer >= 50) { 
                    press_count++;
                    ESP_LOGI("TASK4", "Number of clicks: %lu", press_count);
                    state = STATE_HELD;
                }
                break;

            case STATE_HELD:
                if (!btn_high) {
                    state_timer = now;
                    state = STATE_RELEASE;
                }
                break;

            case STATE_RELEASE:
                if (btn_high) {
                    state = STATE_HELD;
                } else if (now - state_timer >= 50) { 
                    state = STATE_IDLE;
                }
                break;
        }


        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
