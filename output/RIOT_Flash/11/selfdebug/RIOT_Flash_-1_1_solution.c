#include "nvs.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include <stdio.h>  
#include <stdint.h>

#define STORAGE_NAMESPACE "storage"
#define GPIO_OUTPUT_PIN 2

void init_gpio(void) {
    gpio_pad_select_gpio(GPIO_OUTPUT_PIN);
    gpio_set_direction(GPIO_OUTPUT_PIN, GPIO_MODE_OUTPUT);
}

void app_main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    int8_t device_state = 0;
    size_t required_size = sizeof(device_state);
    err = nvs_get_i8(my_handle, "device_state", &device_state);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        device_state = 0;
    }

    init_gpio();

    const esp_timer_create_args_t timer_args = {
        .callback = &update_device_state,
        .arg = &my_handle,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "state_timer"
    };

    esp_timer_handle_t timer;
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer, 1000000));

    while (1) {
        gpio_set_level(GPIO_OUTPUT_PIN, device_state);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    nvs_close(my_handle);
}

void update_device_state(void* arg) {
    nvs_handle_t* my_handle = (nvs_handle_t*)arg;
    int8_t device_state = 0;
    size_t required_size = sizeof(device_state);
    nvs_get_i8(*my_handle, "device_state", &device_state);
    device_state = !device_state;
    nvs_set_i8(*my_handle, "device_state", device_state);
    nvs_commit(*my_handle);
}