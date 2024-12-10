#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <mutex.h>

#define STORAGE_NAMESPACE "storage"

void perform_storage_task(nvs_handle_t *handle) {
    int32_t state = 0;
    while (1) {
        state = !state;  // Toggle state between 1 and 0
        esp_err_t err = nvs_set_i32(*handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error (%s) setting state!\n", esp_err_to_name(err));
            continue;
        }
        err = nvs_commit(*handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing updates!\n", esp_err_to_name(err));
            continue;
        }
        printf("State %d saved to NVS.\n", state);
        xtimer_sleep(1);  // Sleep for 1 second before repeating
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated or has a new version, erase it
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle));
    perform_storage_task(&my_handle);
    nvs_close(my_handle);
    return 0;
}