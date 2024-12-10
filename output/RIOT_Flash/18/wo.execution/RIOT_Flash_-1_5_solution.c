#include "xtimer.h"
#include "unistd.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "msg.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "thread.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "event/thread.h"
#include "mbox.h"
#include "timex.h"
#include "netinet/in.h"
#include "evtimer_mbox.h"

#define STORAGE_NAMESPACE "storage"
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second

nvs_handle_t my_handle;

void persist_and_update_state(void) {
    static int8_t state = 0;
    esp_err_t err;

    // Rotate the device state between 0 and 1
    state = !state;

    // Persist the new state to NVS
    err = nvs_set_i8(my_handle, "state", state);
    if (err != ESP_OK) {
        printf("Error (%s) setting NVS state!\n", esp_err_to_name(err));
        return;
    }

    // Commit to NVS
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
    }

    printf("State is updated and persisted to: %d\n", state);
}

int main(void) {
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS Flash init failed (%s)!\n", esp_err_to_name(err));
        return 1;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return 1;
    }

    printf("NVS Flash initialized and handle opened.\n");

    // Timer loop to update and persist state
    while (1) {
        persist_and_update_state();
        usleep(TIMER_INTERVAL);
    }

    // Close NVS handle (not reached in this example)
    //nvs_close(my_handle);

    return 0;
}