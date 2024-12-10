#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

nvs_handle_t my_handle;

void store_device_state(int state) {
    esp_err_t err;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return;
    }

    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error (%d) setting state to NVS!\n", err);
    } else {
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing to NVS!\n", err);
        }
    }
    nvs_close(my_handle);
}

void *device_state_thread(void *arg) {
    int state = 0;
    while (1) {
        store_device_state(state);
        state = !state;  // Toggle state between 1 and 0
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, device_state_thread, NULL, "device_state_thread");

    return 0;
}