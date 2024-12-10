#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <stdatomic.h>
#include <unistd.h>
#include <thread.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>

#define STORAGE_NAMESPACE "storage"
#define KEY "device_state"
#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define SLEEP_INTERVAL (1U) // 1 second

char stack[STACKSIZE];

void toggle_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    int32_t state = 0;
    err = nvs_get_i32(my_handle, KEY, &state);
    if (err != ESP_OK) {
        state = 0; // Default state
    }

    // Toggle state
    state = !state;

    // Save the state
    err = nvs_set_i32(my_handle, KEY, state);
    ESP_ERROR_CHECK(err);

    // Commit the value
    err = nvs_commit(my_handle);
    ESP_ERROR_CHECK(err);

    // Close NVS handle
    nvs_close(my_handle);
}

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        toggle_state();
        usleep(SLEEP_INTERVAL * 1000000UL); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    printf("Starting device state toggle application...\n");

    thread_create(
        stack,
        sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread_func,
        NULL,
        "toggle_thread"
    );

    return 0;
}