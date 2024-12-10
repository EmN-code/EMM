#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdatomic.h>
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "xtimer.h"
#include "mutex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "arpa/inet.h"

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"

void check_nvs_err(esp_err_t err, const char *msg) {
    if (err != ESP_OK) {
        printf("Error (%s): %s\n", esp_err_to_name(err), msg);
        abort();
    }
}

void write_state_to_nvs(nvs_handle_t nvs_handle, uint8_t state) {
    esp_err_t err = nvs_set_u8(nvs_handle, STATE_KEY, state);
    if (err != ESP_OK) check_nvs_err(err, "Failed to set key");

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) check_nvs_err(err, "Failed to commit");
}

uint8_t read_state_from_nvs(nvs_handle_t nvs_handle) {
    uint8_t state = 0;
    esp_err_t err = nvs_get_u8(nvs_handle, STATE_KEY, &state);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) check_nvs_err(err, "Failed to get key");
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
      ESP_ERROR_CHECK(err);
    }

    nvs_handle_t nvs_handle;
    while ((err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle)) != ESP_OK) {
        printf("Failed to open NVS handle! Retrying...\n");
        xtimer_sleep(1);
    }

    uint8_t state = read_state_from_nvs(nvs_handle);
    printf("Initial device state: %u\n", state);

    while (1) {
        state = !state; // Toggle state between 0 and 1
        write_state_to_nvs(nvs_handle, state);
        printf("Stored device state: %u\n", state);
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}