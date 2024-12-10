#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define STORAGE_NAMESPACE "storage"

dev_t device_state = 0;

void init_nvs_storage() {
    esp_err_t err = nvs_flash_init();
    while (err != ESP_OK) {
        printf("NVS Flash Init failed! Retrying...\n");
        nvs_flash_erase();
        err = nvs_flash_init();
        xtimer_sleep(1);
    }
    printf("NVS Flash Init successful\n");
}

void store_device_state(nvs_handle_t my_handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, "device_state", state);
    while (err != ESP_OK) {
        printf("Error (%s) storing device_state! Retrying...\n", esp_err_to_name(err));
        err = nvs_set_u8(my_handle, "device_state", state);
        xtimer_sleep(1);
    }
}

uint8_t get_stored_device_state(nvs_handle_t my_handle) {
    uint8_t state = 0;
    size_t required_size = sizeof(state);
    esp_err_t err = nvs_get_u8(my_handle, "device_state", &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        printf("Error (%s) reading device_state\n", esp_err_to_name(err));
    }
    return state;
}

int main(void) {
    nvs_handle_t my_handle;
    
    // Initialize the NVS flash
    init_nvs_storage();

    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        printf("Error (%s) opening NVS handle! Retrying...\n", esp_err_to_name(err));
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        xtimer_sleep(1);
    }
    
    device_state = get_stored_device_state(my_handle);

    // Main loop
    while (1) {
        device_state = !device_state; // Toggles state between 0 and 1
        store_device_state(my_handle, device_state);
        
        esp_err_t commit_err = nvs_commit(my_handle);
        while (commit_err != ESP_OK) {
            printf("Error (%s) committing changes! Retrying...\n", esp_err_to_name(commit_err));
            commit_err = nvs_commit(my_handle);
            xtimer_sleep(1);
        }

        printf("Stored device state: %d\n", device_state);

        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}
