#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"


#define BTN_PIN GPIO_NUM_8


static volatile uint32_t press_count = 0;


static void IRAM_ATTR button_isr_handler(void* arg) {
    press_count++;
}


void app_main(void)
{
	gpio_config_t btn_conf = {
        .pin_bit_mask = (1ULL << BTN_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE
	    };
		
		gpio_config(&btn_conf);

		
	    gpio_install_isr_service(0);
	    gpio_isr_handler_add(BTN_PIN, button_isr_handler, NULL);



	    while (true) {
	        vTaskDelay(pdMS_TO_TICKS(1000));
	        ESP_LOGI("TASK1", "All of interrupts: %lu", press_count);
	    }
}
