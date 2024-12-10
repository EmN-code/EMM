#include <xtimer.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <stdio.h>

#define STORAGE_NAMESPACE "storage"
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second

static nvs_handle_t my_handle;
static int32_t device_state = 0;

void store_state_in_nvs(void) {
    esp_err_t err;
    // Store the state in NVS
    err = nvs_set_i32(my_handle, "device_state", device_state);
    if (err != ESP_OK) {
        printf("Failed to set device state!\n");
    } else {
        printf("Stored device state: %ld\n", device_state);
        // Commit the state written to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit device state!\n");
        }
    }
    // Toggle state for next interval
    device_state = !device_state;
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash init failed!\n");
        return -1;
    }

    // Open NVS
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", (int)err);
        return -1;
    }

    // Get initial device state from NVS
    int32_t temp;
    err = nvs_get_i32(my_handle, "device_state", &temp);
    switch (err) {
        case ESP_OK:
            device_state = temp;
            printf("Initial device state: %ld\n", device_state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("Device state not set yet, defaulting to 0.\n");
            device_state = 0;
            break;
        default:
            printf("Error (%d) reading!\n", (int)err);
            return -1;
    }

    // Set timer for storing state every second
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        store_state_in_nvs();
    }

    return 0;
}