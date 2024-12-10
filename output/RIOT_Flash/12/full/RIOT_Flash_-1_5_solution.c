#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <time_units.h>

void handle_nvs_error(esp_err_t err) {
    switch (err) {
        case ESP_ERR_NVS_NOT_FOUND:
            printf("Error: Value not found\n");
            break;
        case ESP_ERR_NVS_INVALID_HANDLE:
            printf("Error: Invalid handle\n");
            break;
        case ESP_ERR_NVS_READ_ONLY:
            printf("Error: Read only mode\n");
            break;
        default:
            printf("NVS Error: %s\n", esp_err_to_name(err));
            break;
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            handle_nvs_error(err);
            return 1;
        }
        err = nvs_flash_init();
        if (err != ESP_OK) {
            handle_nvs_error(err);
            return 1;
        }
    }
    if (err != ESP_OK) {
        handle_nvs_error(err);
        return 1;
    }

    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        handle_nvs_error(err);
        return 1;
    }

    int32_t device_state = 1; // Initial device state
    while (1) {
        err = nvs_set_i32(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            handle_nvs_error(err);
            nvs_close(my_handle);
            return 1;
        }

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            handle_nvs_error(err);
            nvs_close(my_handle);
            return 1;
        }

        ztimer_sleep(ZTIMER_SEC, 1);
        device_state = !device_state; // Toggle state between 1 and 0
    }

    nvs_close(my_handle);
    return 0;
}