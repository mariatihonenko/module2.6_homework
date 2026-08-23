#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_timer.h"

#define BTN_PIN GPIO_NUM_8
#define DEBOUNCE_TIME_US 50000

static volatile bool button_pressed = false;

static void IRAM_ATTR button_isr_handler(void* arg) {
    button_pressed = true;
}



void app_main(void)
{
	gpio_config_t btn_conf = {
	        .pin_bit_mask = (1ULL << BTN_PIN),
	        .mode = GPIO_MODE_INPUT,
	        .pull_up_en = GPIO_PULLUP_DISABLE,
	        .pull_down_en = GPIO_PULLDOWN_ENABLE,
	        .intr_type = GPIO_INTR_POSEDGE 
	    };
    gpio_config(&btn_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(BTN_PIN, button_isr_handler, NULL);

    uint32_t press_count = 0;
    int64_t last_isr_time = 0;

    while (true) {
        if (button_pressed) {
            button_pressed = false; 

            int64_t now = esp_timer_get_time();
            
         
            if ((now - last_isr_time) >= DEBOUNCE_TIME_US) {
                press_count++;
                last_isr_time = now;
                ESP_LOGI("TASK2", "Number of clicks: %lu", press_count);
            }
        }


        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
