
#include "net/gcoap.h"
#include "esp_system.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "nvs_flash.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "nvs.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "string.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "esp_wifi.h"
#include "net/sock/udp.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static nvs_handle_t nvs_handle;

void update_state(void) {
    uint8_t state = (esp_random() % 2); // Simulate state change
    esp_err_t err = nvs_set_u8(nvs_handle, NVS_KEY, state);
    if (err != ESP_OK) {
        printf("Error setting NVS value: %s\n", esp_err_to_name(err));
        return;
    }
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Error committing NVS value: %s\n", esp_err_to_name(err));
    }
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        update_state();
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}
