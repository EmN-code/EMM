#include "ztimer.h"
#include "xtimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdint.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "stdio.h"

// Define the storage namespace and key
#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"

void check_error_and_retry(esp_err_t err, const char *message) {
    while (err != ESP_OK) {
        printf("%s failed with error: %s. Retrying...\n", message, esp_err_to_name(err));
        xtimer_sleep(1); // sleep for 1 second before retrying
        err = nvs_flash_init();
        if (err == ESP_OK) break; // Exit loop if successful
    }
}

void store_device_state(uint8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    check_error_and_retry(err, "NVS open");

    err = nvs_set_u8(my_handle, STATE_KEY, state);
    check_error_and_retry(err, "NVS set_u8");

    err = nvs_commit(my_handle);
    check_error_and_retry(err, "NVS commit");

    nvs_close(my_handle);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    check_error_and_retry(err, "NVS flash init");

    uint8_t state = 0;

    while (1) {
        // Toggle the state between 0 and 1
        state = (state == 0) ? 1 : 0;

        // Store the state in NVS
        store_device_state(state);

        printf("Stored device state: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    return 0;
}