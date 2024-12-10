
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time_units.h>
#include <timex.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <stdatomic.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;
static mutex_t nvs_mutex = MUTEX_INIT;

void store_device_state(uint8_t state) {
    mutex_lock(&nvs_mutex);
    nvs_set_u8(my_handle, DEVICE_STATE_KEY, state);
    nvs_commit(my_handle);
    mutex_unlock(&nvs_mutex);
}

uint8_t read_device_state(void) {
    uint8_t state;
    mutex_lock(&nvs_mutex);
    nvs_get_u8(my_handle, DEVICE_STATE_KEY, &state);
    mutex_unlock(&nvs_mutex);
    return state;
}

int main(void) {
    int err;
    uint8_t device_state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash init failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    while (1) {
        // Simulate temperature update
        device_state = (device_state == 0) ? 1 : 0;

        // Store the updated state in NVS
        store_device_state(device_state);

        // Read and print the current state from NVS
        printf("Current Device State: %d\n", read_device_state());

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);

    return 0;
}
