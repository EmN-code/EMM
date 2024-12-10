#include <nvs_flash.h>
#include <nvs.h>
#include "xtimer.h"
#include "stdio.h"

#define STORAGE_NAMESPACE "storage"
#define TEMPERATURE_KEY "temperature"

void nvs_init(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void save_temperature(int temperature) {
    nvs_handle_t my_handle;
    // Open NVS handle
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        // Write temperature to NVS
        err = nvs_set_i32(my_handle, TEMPERATURE_KEY, temperature);
        if (err == ESP_OK) {
            nvs_commit(my_handle);
            printf("Temperature saved: %d\n", temperature);
        } else {
            printf("Failed to write temperature to NVS. Error (%d)\n", err);
        }
        // Close NVS handle
        nvs_close(my_handle);
    }
}

int main(void) {
    nvs_init();

    int temperature = 0;

    while (1) {
        // Simulate temperature reading
        temperature = 1 - temperature; // Toggles between 0 and 1

        // Save temperature to NVS every second
        save_temperature(temperature);

        xtimer_sleep(1);
    }
    return 0;
}