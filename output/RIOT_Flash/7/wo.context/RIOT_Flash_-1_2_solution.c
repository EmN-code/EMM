#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_err.h>
#include <esp_system.h>
#include <xtimer.h>

#define STORAGE_NAMESPACE "storage"
#define KEY_TEMPERATURE "temperature"
#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

void update_temperature(nvs_handle_t handle) {
    int32_t temperature = esp_random() % 100;  // Simulate a temperature reading
    esp_err_t err = nvs_set_i32(handle, KEY_TEMPERATURE, temperature);
    if (err != ESP_OK) {
        printf("Error (%s) setting temperature!\n", esp_err_to_name(err));
    } else {
        printf("Temperature updated to: %ld\n", temperature);
        nvs_commit(handle);
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        while (1) {
            update_temperature(my_handle);
            xtimer_sleep(INTERVAL);
        }
        nvs_close(my_handle);
    }
    return 0;
}