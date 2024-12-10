#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"
#include "string.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"

#define STORAGE_NAMESPACE "storage"

void store_device_state(int state) {
    nvs_handle my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write the state to NVS
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    }

    // Commit the changes
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
    }

    // Close NVS handle
    nvs_close(my_handle);
}

int main(void) {
    int state = 0;
    while (1) {
        // Toggle state between 1 and 0
        state = !state;

        // Store the current state in NVS
        store_device_state(state);

        // Wait for 1 second before updating again
        xtimer_sleep(1);
    }

    return 0;
}