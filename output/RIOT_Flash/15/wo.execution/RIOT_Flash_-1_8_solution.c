#include <ztimer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <timex.h>
#include <mutex.h>
#include <thread.h>
#include <xtimer.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define SLEEP_INTERVAL           (1000000) // 1 second in microseconds

// Device state as a global variable for simplicity. Normally, consider encapsulation.
static int device_state = 0;
static char thread_stack[THREAD_STACKSIZE_MAIN];

// Thread function to periodically update NVS
static void *update_nvs(void *arg) {
    (void)arg;
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return NULL;
    }

    while (1) {
        // Store updated device state in NVS
        device_state = (device_state + 1) % 2;  // Toggle state between 0 and 1
        err = nvs_set_i32(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Error (%d) setting NVS value!\n", err);
        }
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Commit failed with error (%d)!\n", err);
        }

        printf("Stored device state: %d\n", device_state);

        // Wait for next update
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL/1000);
    }

    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Initialize thread for NVS update
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, update_nvs, NULL, "nvs_updater");

    return 0;
}