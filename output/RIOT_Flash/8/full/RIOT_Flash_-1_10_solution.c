#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "mbox.h"
#include "msg.h"
#include "kernel_defines.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

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
        printf("Error (%s) opening NVS!
", esp_err_to_name(err));
        return err;
    }

    int32_t device_state = 0;

    while (1) {
        device_state = (device_state == 0) ? 1 : 0;
        err = nvs_set_i32(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Error (%s) writing!
", esp_err_to_name(err));
            continue;
        }

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing!
", esp_err_to_name(err));
            continue;
        }

        printf("Stored device state: %d
", device_state);
        usleep(1000000); // Sleep for 1 second
    }

    nvs_close(my_handle);
    return 0;
}