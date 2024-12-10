#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "unistd.h"

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "state"
#define DELAY_INTERVAL (1LU * US_PER_SEC)

static void write_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        err = nvs_set_i32(my_handle, STATE_KEY, state);
        if (err != ESP_OK) {
            printf("Error (%d) writing state to NVS!\n", err);
        } else {
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%d) committing updates to NVS!\n", err);
            }
        }
        nvs_close(my_handle);  // Correct closure of NVS handle
    }
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init_partition_ptr(NULL);
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS flash!\n", err);
        return err;
    }

    int device_state = 0;
    while (1) {
        write_device_state(device_state);

        // Toggle the device state for the next cycle
        device_state = !device_state;

        // Wait for the specified interval
        ztimer_sleep(ZTIMER_MSEC, DELAY_INTERVAL);
    }

    return 0;
}