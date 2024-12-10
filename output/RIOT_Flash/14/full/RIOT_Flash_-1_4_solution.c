#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <mbox.h>
#include <msg.h>
#include <nbuf.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

int main(void) {
    esp_err_t err;
    nvs_handle_t my_handle;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS\n", esp_err_to_name(err));
        return -1;
    }

    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return -1;
    }

    uint8_t device_state = 0;

    while (1) {
        err = nvs_set_u8(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Failed to write!\n");
        } else {
            printf("Device state %d written successfully.\n", device_state);

            // Toggle device state between 0 and 1
            device_state = !device_state;
        }

        // Commit the written value to NVS
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Failed to commit!\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    // Close NVS handle
    nvs_close(my_handle);
    return 0;
}
