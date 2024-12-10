#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "nvs_flash.h"
#include "nvs.h"

int main(void) {
    nvs_handle_t handle;
    esp_err_t err;

    /* Initialize NVS */
    while(1) {
        err = nvs_flash_init();
        if (err == ESP_OK) {
            break;
        }
        sleep(1);
    }

    /* Open NVS handle */
    while(1) {
        err = nvs_open("storage", NVS_READWRITE, &handle);
        if (err == ESP_OK) {
            break;
        }
        sleep(1);
    }

    /* Main loop */
    uint8_t state = 0;
    while (1) {
        /* Read the state from NVS */
        while(1) {
            err = nvs_get_u8(handle, "device_state", &state);
            if (err == ESP_OK || err == ESP_ERR_NVS_NOT_FOUND) {
                break;
            }
            sleep(1);
        }

        /* Toggle the state */
        state = !state;

        /* Save the updated state back to NVS */
        while(1) {
            err = nvs_set_u8(handle, "device_state", state);
            if (err == ESP_OK) {
                break;
            }
            sleep(1);
        }

        /* Commit to ensure value is written to the storage */
        while(1) {
            err = nvs_commit(handle);
            if (err == ESP_OK) {
                break;
            }
            sleep(1);
        }

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    nvs_close(handle);
    return 0;
}