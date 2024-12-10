 #include <arpa/inet.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <msg.h>
#include <MQTTClient.h>
#include <netinet/in.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <paho_mqtt.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

// Description: This code snippet demonstrates how to initialize and use the Non-Volatile Storage (NVS) on an ESP32 with RIOT OS. It writes a device state ("1" for ON and "0" for OFF) into NVS every second. The code ensures continuous updating of this state.

#define STORAGE_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;
static int temp = 1;

void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS\n");
    }
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle\n");
    }
}

void write_device_state(void) {
    esp_err_t err = nvs_set_i32(nvs_handle, DEVICE_STATE_KEY, temp);
    if (err == ESP_OK) {
        printf("Device state saved successfully\n");
        nvs_commit(nvs_handle);
        temp = !temp; // Toggle state for demonstration
    } else {
        printf("Error saving device state\n");
    }
}

void *state_updater_thread(void *arg) {
    (void)arg;
    while (1) {
        write_device_state();
        sleep(1);
    }
    return NULL;
}

int main(void) {
    init_nvs();
    thread_create(state_updater_thread, NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, "state_updater");

    return 0;
}