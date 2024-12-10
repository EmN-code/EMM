
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <nvs.h>
#include <nvs_flash.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t my_handle;

void update_device_state(bool state) {
    // Simulate updating the device state in NVS
    printf("Updating device state to %d\n", state);
    // In a real scenario, you would use nvs_set_u8 here
}

void read_device_state(void) {
    // Simulate reading the device state from NVS
    printf("Reading device state\n");
    // In a real scenario, you would use nvs_get_u8 here
}

int main(void) {
    int err;
    bool device_state = false;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return err;
    }

    while (1) {
        // Read the current device state
        read_device_state();

        // Toggle the device state
        device_state = !device_state;

        // Update the device state in NVS
        update_device_state(device_state);

        // Sleep for 1 second
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    // Close the NVS handle
    nvs_close(my_handle);

    return 0;
}
