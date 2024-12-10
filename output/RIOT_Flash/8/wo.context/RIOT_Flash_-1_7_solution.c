#include "thread_flags.h"
#include "xtimer.h"
#include "stdio.h"
#include "nvs.h"
#include "nvs_flash.h"

#define DELAY 1U * 1000000U  // 1 second in microseconds

void store_state(nvs_handle_t handle, int state) {
    esp_err_t err;
    err = nvs_set_i32(handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Failed to write state to NVS!\n");
        return;
    }
    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Failed to commit to NVS!\n");
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS!\n");
        return 1;
    }
    
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle!\n");
        return 1;
    }

    int state = 1;

    while (1) {
        store_state(my_handle, state);
        state = !state;  // Toggle state between 0 and 1
        xtimer_usleep(DELAY);
    }

    // Close NVS handle (unreachable in this loop)
    nvs_close(my_handle);

    return 0;
}