#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <dht.h>
#include <paho_mqtt.h>
#include <mbox.h>
#include <MQTTClient.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define SLEEP_DURATION 1  // Sleep duration in seconds

void update_device_state(nvs_handle_t handle) {
    static uint8_t state = 0;
    state = (state == 0) ? 1 : 0;  // Toggle state between 0 and 1
    esp_err_t err = nvs_set_u8(handle, STATE_KEY, state);
    if (err != ESP_OK) {
        printf("Failed to set device state to NVS: %s\n", esp_err_to_name(err));
        return;
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        printf("Failed to commit changes to NVS: %s\n", esp_err_to_name(err));
    }
}

void *state_update_thread(void *arg) {
    (void)arg;
    nvs_handle_t handle;
    if (nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle) != ESP_OK) {
        printf("Failed to open NVS handle\n");
        return NULL;
    }

    while (1) {
        update_device_state(handle);
        xtimer_sleep(SLEEP_DURATION);
    }

    nvs_close(handle);
    return NULL;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0, state_update_thread, NULL, "state_update_thread");

    while (1) {
        // Main thread does nothing, state_update_thread manages the state
    }

    return 0;
}