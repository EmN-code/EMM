#include <nvs_flash.h>
#include <nvs.h>
#include <stdio.h>
#include <esp_timer.h>
#include <esp_system.h>

nvs_handle_t my_handle;
bool device_state = 0; // Start with a state of '0'

// Timer callback function to toggle device state and save it to NVS
void timer_callback(void* arg) {
    esp_err_t err;
    device_state = !device_state; // Toggle the state

    // NVS write operation
    err = nvs_set_u8(my_handle, "device_state", device_state);
    if (err != ESP_OK) {
        printf("Error (%s) saving device_state!\n", esp_err_to_name(err));
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing device_state!\n", esp_err_to_name(err));
    }

    printf("Device state updated and saved: %d\n", device_state);
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    // Initialize timer that ticks every 1 second (1000 ms)
    esp_timer_handle_t periodic_timer;
    esp_timer_create_args_t timer_args = {
        .callback = &timer_callback,
        .name = "periodic_timer"
    };
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1000000));

    while (1) {
        // Main chip tasks can be handled here
        vTaskDelay(pdMS_TO_TICKS(1000)); // Yield for 1 second
    }

    nvs_close(my_handle);
    return 0;
}