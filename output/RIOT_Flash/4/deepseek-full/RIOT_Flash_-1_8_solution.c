
#include "net/sock/udp.h"
#include "mbox.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "esp_system.h"
#include "msg.h"
#include "timex.h"
#include "stddef.h"
#include "unistd.h"
#include "stdio.h"
#include "stdatomic.h"
#include "nvs.h"
#include "net/emcute.h"
#include "mutex.h"
#include "time_units.h"
#include "stdlib.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "string.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "stdbool.h"
#include "ztimer.h"
#include "nvs_flash.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static nvs_handle_t my_handle;

void update_state(void) {
    static uint8_t state = 0;
    state = !state; // Toggle state

    esp_err_t err = nvs_set_u8(my_handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) setting state!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing state!\n", esp_err_to_name(err));
        }
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        while (1) {
            update_state();
            xtimer_sleep(1); // Sleep for 1 second
        }
    }

    nvs_close(my_handle);
    return 0;
}
