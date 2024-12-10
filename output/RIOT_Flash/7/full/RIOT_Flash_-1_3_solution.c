#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "string.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

// Function prototypes
err_t init_nvs_flash(void);
err_t open_nvs_handle(nvs_handle_t *my_handle);
err_t write_state_to_nvs(nvs_handle_t my_handle, uint8_t state);
err_t read_state_from_nvs(nvs_handle_t my_handle, uint8_t *state);

int main(void) {
    nvs_handle_t my_handle;
    uint8_t state = 0;  // Initial state
    esp_err_t err;

    // Initialize NVS Flash
    err = init_nvs_flash();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS flash!\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS Handle
    err = open_nvs_handle(&my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }

    while (true) {
        // Toggle state between 0 and 1
        state = !state;

        // Write state to NVS
        err = write_state_to_nvs(my_handle, state);
        if (err != ESP_OK) {
            printf("Error (%s) writing state to NVS!\n", esp_err_to_name(err));
        }
        
        // Read state from NVS
        err = read_state_from_nvs(my_handle, &state);
        if (err != ESP_OK) {
            printf("Error (%s) reading state from NVS!\n", esp_err_to_name(err));
        } else {
            printf("Successfully read state from NVS: %d\n", state);
        }

        // Sleep for one second
        usleep(1000000);
    }

    // Commit and close NVS handle
    nvs_commit(my_handle);
    nvs_close(my_handle);

    return 0;
}

err_t init_nvs_flash() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("Erasing NVS flash...\n");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    return err;
}

err_t open_nvs_handle(nvs_handle_t *my_handle) {
    esp_err_t err = nvs_open("storage", NVS_READWRITE, my_handle);
    return err;
}

err_t write_state_to_nvs(nvs_handle_t my_handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(my_handle, "state", state);
    return err;
}

err_t read_state_from_nvs(nvs_handle_t my_handle, uint8_t *state) {
    esp_err_t err = nvs_get_u8(my_handle, "state", state);
    return err;
}