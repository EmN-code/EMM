#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "sys/socket.h"
#include "unistd.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "mbox.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "esp_wifi.h"

#define STORAGE_NAMESPACE "storage"

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initialising NVS\n", esp_err_to_name(err));
        return err;
    }

    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    int32_t state = 0;
    while (1) {
        err = nvs_set_i32(my_handle, "state", state);
        if (err == ESP_OK) {
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing updates to NVS!\n", esp_err_to_name(err));
            } else {
                printf("State %ld written successfully!\n", (long)state);
            }
        } else {
            printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
        }
        // Toggle state between 0 and 1
        state = !state;

        // Sleep for 1 second
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    // Close NVS handle (unreachable in this code due to while(1) loop)
    // nvs_close(my_handle);

    return 0;
}