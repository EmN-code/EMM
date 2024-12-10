#include <ztimer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <string.h>

#define STORAGE_NAMESPACE "storage"
#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds for ztimer_usec_t

static nvs_handle_t my_handle;
static void update_device_state(void *arg) {
    static uint8_t state = 0;
    esp_err_t err;

    // Toggle the state between 0 and 1
    state = !state;

    // Write the state to NVS
    err = nvs_set_u8(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%d) writing to NVS!\n", err);
    } else {
        nvs_commit(my_handle);
        printf("Device state: %d\n", state);
    }
    
    // Re-trigger the timer for the next update
    ztimer_set(ZTIMER_USEC, arg, TIMER_INTERVAL);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS!\n", err);
        return -1;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return -1;
    }

    // Setup and start timer
    ztimer_t timer;
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);
    ztimer_callback(&timer, update_device_state);
    
    // Keeping the process alive
    while (1) {
        ztimer_sleep(ZTIMER_USEC, TIMER_INTERVAL);
    }

    return 0;
}