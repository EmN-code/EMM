
#include <esp_wifi.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <string.h>
#include <stddef.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <esp_system.h>
#include <time_units.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <nvs_flash.h>
#include <sys/socket.h>
#include <nvs.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <stdio.h>

void store_temperature(nvs_handle_t handle, int32_t temp) {
    esp_err_t err = nvs_set_i32(handle, "temperature", temp);
    if (err != ESP_OK) {
        printf("Error writing temperature to NVS: %d\n", err);
    } else {
        err = nvs_commit(handle);
        if (err != ESP_OK) {
            printf("Error committing to NVS: %d\n", err);
        }
    }
}

int main() {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return err;
    }

    while (1) {
        // Simulate temperature reading
        int32_t temp = rand() % 100; // Random temperature between 0 and 99

        // Store temperature in NVS
        store_temperature(my_handle, temp);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}
