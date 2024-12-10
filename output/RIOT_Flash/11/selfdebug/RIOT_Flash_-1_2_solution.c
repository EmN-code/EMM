#include <nvs_flash.h>
#include <nvs.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>

#define STORAGE_NAMESPACE "storage"
#define GPIO_OUTPUT_PIN   2

void update_device_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, "device_state", state);
    nvs_commit(handle);
}

void nvs_task(void *arg) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        return;
    }

    int device_state = 0;

    // Configure GPIO
    gpio_pad_select_gpio(GPIO_OUTPUT_PIN);
    gpio_set_direction(GPIO_OUTPUT_PIN, GPIO_MODE_OUTPUT);

    while (1) {
        update_device_state(my_handle, device_state);

        // Set GPIO to represent device state
        gpio_set_level(GPIO_OUTPUT_PIN, device_state);

        // Toggle state
        device_state = !device_state;

        // Delay for 1 second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    nvs_close(my_handle);
}

void app_main(void) {
    xTaskCreate(nvs_task, "nvs_task", 4096, NULL, 5, NULL);
}