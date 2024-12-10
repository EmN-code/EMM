#include "xtimer.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "kernel_defines.h"

#define STORAGE_NAMESPACE "storage"

void store_state(nvs_handle_t my_handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Failed to write device state!\n");
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit device state!\n");
    }
}

void main_loop(void) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    while (1) {
        uint8_t state = 0;
        err = nvs_get_u8(my_handle, "device_state", &state);
        if (err == ESP_OK) {
            printf("Current state: %d\n", state);
            state = state == 1 ? 0 : 1;  // Toggle state
        }
        else if (err == ESP_ERR_NVS_NOT_FOUND) {
            printf("Device state not initialized yet, setting to 0.\n");
            state = 0;
        }
        else {
            printf("Error (%d) reading device state!\n", err);
        }
        
        store_state(my_handle, state);

        xtimer_sleep(1);
    }

    nvs_close(my_handle);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    
    main_loop();
    return 0;
}