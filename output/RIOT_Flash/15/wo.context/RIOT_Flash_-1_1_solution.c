#include "xtimer.h"
#include "thread.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval

void toggle_state_in_nvs(void) {
    nvs_handle_t handle;
    esp_err_t err;
    int32_t device_state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash Init failed: %d\n", err);
        return;
    }

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return;
    }

    // Read the current state
    err = nvs_get_i32(handle, "state", &device_state);
    if (err == ESP_OK) {
        printf("Read state: %d\n", device_state);
    } else if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("State not initialized in NVS, setting to 0\n");
        device_state = 0;
    } else {
        printf("Error reading NVS: %d\n", err);
    }

    // Toggle the state
    device_state = !device_state;

    // Write the new state
    err = nvs_set_i32(handle, "state", device_state);
    if (err != ESP_OK) {
        printf("Error writing to NVS: %d\n", err);
    } else {
        printf("Written new state: %d\n", device_state);
        nvs_commit(handle);
    }

    // Close the handle
    nvs_close(handle);
}

void *timer_thread(void *arg) {
    (void)arg;
    while (1) {
        toggle_state_in_nvs();
        xtimer_sleep(TIMER_INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, timer_thread, NULL, "timer");
    return 0;
}