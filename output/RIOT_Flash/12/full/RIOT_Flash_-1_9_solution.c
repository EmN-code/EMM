#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <mutex.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <timex.h>
#include <time_units.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>

#define STORAGE_NAMESPACE "storage"

void initialize_nvs() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void save_state_to_nvs(int32_t state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Failed to write to NVS (%s)!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit updates in NVS (%s)!\n", esp_err_to_name(err));
        }
    }

    nvs_close(my_handle);
}

int main(void) {
    initialize_nvs();
    int32_t state = 0;

    while (1) {
        save_state_to_nvs(state);
        state = 1 - state;  // Toggle state between 0 and 1
        xtimer_sleep(1);  // Wait for 1 second
    }

    return 0;
}