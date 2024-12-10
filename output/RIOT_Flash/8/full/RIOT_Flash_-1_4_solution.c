#include <ztimer.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_system.h>
#include <esp_wifi.h>

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"
#define UPDATE_INTERVAL_SEC (1U)

int main(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return 1;
    }

    nvs_handle_t nvs_handle;

    // Open
    while (true) {
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            ztimer_sleep(ZTIMER_MSEC, UPDATE_INTERVAL_SEC * 1000);
            continue;
        }
        break;
    }

    bool device_state = false;

    while (true) {
        // Write the device state
        err = nvs_set_i32(nvs_handle, DEVICE_STATE_KEY, device_state);
        if (err != ESP_OK) {
            printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
        }

        // Commit written value to storage
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
        }

        // Toggle device state
        device_state = !device_state;

        // Wait for the next update interval
        ztimer_sleep(ZTIMER_MSEC, UPDATE_INTERVAL_SEC * 1000);
    }

    // Close NVS
    nvs_close(nvs_handle);
    return 0;
}
