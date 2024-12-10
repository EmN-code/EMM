#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "string.h"
#include "stdatomic.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "kernel_defines.h"
#include "msg.h"
#include "mutex.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "net/emcute.h"
#include "net/gcoap.h"

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return -1;
    }

    int32_t device_state = 0;

    while (1) {
        // Set a new device state every second
        device_state = (device_state == 0) ? 1 : 0;

        err = nvs_set_i32(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Error (%s) writing!
", esp_err_to_name(err));
        }

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS commit failed!
");
        }

        printf("Device state updated to: %d
", device_state);
        sleep(1); // Sleep for 1 second
    }

    // Close NVS handle (unreachable but demonstrates cleanup)
    nvs_close(my_handle);

    return 0;
}