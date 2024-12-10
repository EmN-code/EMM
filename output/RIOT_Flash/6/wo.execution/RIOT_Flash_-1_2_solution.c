#include <unistd.h>
#include <sys/socket.h>
#include <time_units.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open handle
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    // Device state variables
    uint8_t device_state = 0;

    while (1) {
        // Write value to NVS
        err = nvs_set_u8(my_handle, "device_state", device_state);
        ESP_ERROR_CHECK(err);

        // Commit written value.
        err = nvs_commit(my_handle);
        ESP_ERROR_CHECK(err);

        // Toggle device state between 0 and 1
        device_state = !device_state;

        // Delay for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Close
    nvs_close(my_handle);
}

int main(void) {
    app_main();
    return 0;
}