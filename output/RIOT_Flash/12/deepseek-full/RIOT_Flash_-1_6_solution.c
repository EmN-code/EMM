
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <net/gcoap.h>
#include <ztimer.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <nvs.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"
#define DEVICE_STATE_ON 1
#define DEVICE_STATE_OFF 0

static nvs_handle_t nvs_handle;

void toggle_device_state(void)
{
    uint8_t state;
    esp_err_t err;

    err = nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    if (err != ESP_OK) {
        printf("Failed to read device state from NVS: %s\n", esp_err_to_name(err));
        return;
    }

    state = (state == DEVICE_STATE_ON) ? DEVICE_STATE_OFF : DEVICE_STATE_ON;

    err = nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to write device state to NVS: %s\n", esp_err_to_name(err));
        return;
    }

    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to commit device state to NVS: %s\n", esp_err_to_name(err));
        return;
    }

    printf("Device state toggled to: %s\n", state == DEVICE_STATE_ON ? "ON" : "OFF");
}

int main(void)
{
    esp_err_t err;

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            printf("Failed to erase NVS flash: %s\n", esp_err_to_name(err));
            return 1;
        }
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("Failed to initialize NVS flash: %s\n", esp_err_to_name(err));
        return 1;
    }

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle: %s\n", esp_err_to_name(err));
        return 1;
    }

    while (1) {
        toggle_device_state();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    nvs_close(nvs_handle);
    return 0;
}
