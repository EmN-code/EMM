#include "xtimer.h"
#include "stdlib.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_system.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "msg.h"
#include "mutex.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "time_units.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "esp_wifi.h"

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    while (err != ESP_OK) {
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
        }
        printf("NVS flash Init error: retrying in 1 second\n");
        xtimer_sleep(1);
    }

    // Open NVS handle
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        xtimer_sleep(1);
        continue;
    }
    printf("NVS Opened\n");

    // Write to NVS
    int32_t temp = 25; // Simulate temperature reading
    err = nvs_set_i32(my_handle, "temperature", temp);
    while (err != ESP_OK) {
        printf("Error (%s) setting NVS! Retrying in 1 second.\n", esp_err_to_name(err));
        xtimer_sleep(1);
        err = nvs_set_i32(my_handle, "temperature", temp);
    }

    // Commit written value
    err = nvs_commit(my_handle);
    while (err != ESP_OK) {
        printf("Error (%s) committing NVS! Retrying in 1 second.\n", esp_err_to_name(err));
        xtimer_sleep(1);
        err = nvs_commit(my_handle);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}