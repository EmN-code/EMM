
#include <stdlib.h>
#include <nvs_flash.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <stdio.h>
#include <stdatomic.h>
#include <esp_wifi.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <nvs.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <net/sock/udp.h>
#include <string.h>

void store_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, "device_state", state);
    nvs_commit(handle);
}

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int state = 1; // Initial state

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return -1;
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return -1;
    }

    while (1) {
        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Store state in NVS
        store_state(my_handle, state);

        // Print current state
        printf("Current state: %d\n", state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS (this will never be reached in this example)
    nvs_close(my_handle);

    return 0;
}
