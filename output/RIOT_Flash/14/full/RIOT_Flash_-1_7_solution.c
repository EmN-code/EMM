#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <xtimer.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <stdatomic.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <unistd.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define NVS_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define SLEEP_INTERVAL (1U)

static nvs_handle_t nvs_handle;

void store_device_state(bool state) {
    esp_err_t err;
    while (1) {
        err = nvs_set_u8(nvs_handle, STATE_KEY, (uint8_t)state);
        if (err != ESP_OK) {
            printf("Error (%s) setting state! Retrying...\n", esp_err_to_name(err));
            xtimer_sleep(SLEEP_INTERVAL);
            continue;
        }
        err = nvs_commit(nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing state! Retrying...\n", esp_err_to_name(err));
            xtimer_sleep(SLEEP_INTERVAL);
            continue;
        }
        break;
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
    ESP_ERROR_CHECK(err);

    bool device_state = false;

    while (1) {
        device_state = !device_state;  // toggle state
        store_device_state(device_state);
        xtimer_sleep(SLEEP_INTERVAL);
    }

    nvs_close(nvs_handle);
    return 0;
}