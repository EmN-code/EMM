
#include <esp_wifi.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <string.h>
#include <stddef.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <esp_system.h>
#include <time_units.h>
#include <unistd.h>
#include <net/gcoap.h>
#include <mbox.h>
#include <netinet/in.h>
#include <evtimer_msg.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <nvs_flash.h>
#include <sys/socket.h>
#include <nvs.h>
#include <xtimer.h>
#include <ztimer.h>
#include <net/emcute.h>
#include <stdio.h>

void store_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, "device_state", state);
    nvs_commit(handle);
}

int main() {
    esp_err_t err;
    nvs_handle_t my_handle;
    int state = 1; // Example state

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return err;
    }

    // Open NVS
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return err;
    }

    while (1) {
        // Store the state
        store_state(my_handle, state);

        // Toggle state
        state = (state == 1) ? 0 : 1;

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS (this will never be reached in this example)
    nvs_close(my_handle);

    return 0;
}
