
#include "esp_system.h"
#include "driver/gpio.h"

#define GPIO_PIN_21 GPIO_NUM_21
#define GPIO_PIN_22 GPIO_NUM_22

void delay(uint32_t milliseconds) {
    vTaskDelay(milliseconds / portTICK_PERIOD_MS);
}

int main(void) {
    // Initialize GPIO pins
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << GPIO_PIN_21) | (1ULL << GPIO_PIN_22);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    while (1) {
        // Toggle GPIO pins
        gpio_set_level(GPIO_PIN_21, 1);
        gpio_set_level(GPIO_PIN_22, 0);
        delay(5000); // 5 seconds delay

        gpio_set_level(GPIO_PIN_21, 0);
        gpio_set_level(GPIO_PIN_22, 1);
        delay(5000); // 5 seconds delay
    }

    return 0;
}
