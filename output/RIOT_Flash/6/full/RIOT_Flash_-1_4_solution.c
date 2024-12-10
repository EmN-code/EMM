#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <string.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <kernel_defines.h>

#define NAMESPACE "storage"
#define STATE_KEY "device_state"

void error_handler(esp_err_t err) {
    if (err != ESP_OK) {
        printf("NVS operation failed with error %d\n", err);
    }
}

void save_device_state(int state) {
    nvs_handle_t handle;
    esp_err_t err;

    err = nvs_open(NAMESPACE, NVS_READWRITE, &handle);
    error_handler(err);

    err = nvs_set_i32(handle, STATE_KEY, state);
    error_handler(err);

    err = nvs_commit(handle);
    error_handler(err);

    nvs_close(handle);
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    error_handler(err);

    int state = 0;

    while (1) {
        save_device_state(state);
        printf("Device state saved as: %d\n", state);

        state = (state == 0) ? 1 : 0; // Toggle state between 0 and 1

        xtimer_sleep(1);
    }

    return 0;
}