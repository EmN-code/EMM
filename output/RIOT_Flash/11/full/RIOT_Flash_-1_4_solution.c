#include <ztimer.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_system.h>

#define TIMER_INTERVAL (1U * 1000000U)  // 1 second in microseconds

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS\n");
        return -1;
    }

    nvs_handle_t nvs_handle;
    err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle\n");
       return -1;
    }

    int32_t state = 0;
    while (1) {
        // Toggle state
        state = (state == 0) ? 1 : 0;

        // Write state to NVS
        err = nvs_set_i32(nvs_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Failed to write to NVS\n");
        } else {
            printf("Device state %d saved to NVS\n", state);
            nvs_commit(nvs_handle);
        }

        // Sleep for 1 second
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    
    // Code should never reach here as the loop runs indefinitely
    nvs_close(nvs_handle);
    return 0;
}