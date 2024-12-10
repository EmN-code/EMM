
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;
static mutex_t nvs_mutex = MUTEX_INIT;

void update_device_state(uint8_t state) {
    mutex_lock(&nvs_mutex);
    nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    nvs_commit(nvs_handle);
    mutex_unlock(&nvs_mutex);
}

uint8_t read_device_state(void) {
    uint8_t state = 0;
    mutex_lock(&nvs_mutex);
    nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state);
    mutex_unlock(&nvs_mutex);
    return state;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    ESP_ERROR_CHECK(err);

    while (1) {
        uint8_t current_state = read_device_state();
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        update_device_state(new_state);
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}
