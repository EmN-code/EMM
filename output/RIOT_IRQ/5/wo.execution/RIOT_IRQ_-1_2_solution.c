
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <esp_intr_alloc.h>
#include <esp_system.h>
#include <esp_log.h>

static volatile bool interrupt_occurred = false;
static volatile bool interrupt_handled = false;

static void IRAM_ATTR interrupt_handler(void *arg)
{
    interrupt_occurred = true;
    interrupt_handled = true;
}

int main(void)
{
    esp_err_t err;
    uint32_t interrupt_pin = 4; // Example pin, adjust as needed

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Allocate interrupt
    err = esp_intr_alloc(ETS_GPIO_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, interrupt_handler, NULL, NULL);
    if (err != ESP_OK) {
        ESP_LOGE("Interrupt", "Failed to allocate interrupt: %d", err);
        return -1;
    }

    // Enable GPIO interrupt
    gpio_install_isr_service(0);
    gpio_set_intr_type(interrupt_pin, GPIO_INTR_POSEDGE);
    gpio_intr_enable(interrupt_pin);

    // Main loop
    while (1) {
        if (interrupt_occurred) {
            if (interrupt_handled) {
                ESP_LOGI("Interrupt", "Interrupt handled correctly");
                interrupt_handled = false;
            } else {
                ESP_LOGE("Interrupt", "Interrupt occurred but not handled");
            }
            interrupt_occurred = false;
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }

    return 0;
}
