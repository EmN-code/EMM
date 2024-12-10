
#include "nvs_flash.h"
#include "msg.h"
#include "time_units.h"
#include "esp_system.h"
#include "xtimer.h"
#include "stdio.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "esp_wifi.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "nvs.h"
#include "sys/socket.h"
#include "stddef.h"
#include "mbox.h"

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void update_device_state(void)
{
    static int state = 0;
    state = !state; // Toggle state

    esp_err_t err = nvs_set_i8(my_handle, DEVICE_STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Error (%s) setting device state!\n", esp_err_to_name(err));
    } else {
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%s) committing device state!\n", esp_err_to_name(err));
        } else {
            printf("Device state updated to %d\n", state);
        }
    }
}

int main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        while (1) {
            update_device_state();
            xtimer_sleep(1); // Sleep for 1 second
        }
    }

    nvs_close(my_handle);
    return 0;
}
