
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>
#include <esp_err.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

void update_device_state(nvs_handle_t handle, int state) {
    esp_err_t err = nvs_set_i32(handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting device state: %d\n", err);
        return;
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Error committing NVS: %d\n", err);
    }
}

int main(void) {
    nvs_handle_t handle;
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash init failed: %d\n", err);
        return 1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    while (1) {
        int current_state = (xtimer_now().ticks32 % 2); // Simulate state change
        update_device_state(handle, current_state);
        printf("Device state updated to: %d\n", current_state);
        xtimer_sleep(1); // Wait for 1 second
    }

    nvs_close(handle);
    return 0;
}
