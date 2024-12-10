#include "xtimer.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "time_units.h"
#include "timex.h"
#include "time.h"
#include "unistd.h"
#include "ztimer.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define DELAY_ONE_SECOND (1U * US_PER_SEC)

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    uint8_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    // Verify that NVS is properly initialized
    if (err != ESP_OK) {
        printf("Error initializing NVS!\n");
        return 1;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!\n");
        return 1;
    }

    // Continuously write and update device state every second
    while (1) {
        // Set the new device state
        err = nvs_set_u8(my_handle, STATE_KEY, state);
        if (err != ESP_OK) {
            printf("Failed to set device state!\n");
        } else {
            // Commit to save changes
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Failed to commit NVS changes!\n");
            }
        }

        // Toggle state for demonstration
        state = !state;

        // Delay for 1 second
        xtimer_usleep(DELAY_ONE_SECOND);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}