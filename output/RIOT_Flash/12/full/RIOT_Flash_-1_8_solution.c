#include "ztimer.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "msg.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "mutex.h"
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"
#define DELAY_SECONDS (1U)

int main(void) {
    nvs_flash_init();
    nvs_handle_t my_handle;
    
    while (1) {
        // Attempt to open NVS with read/write access
        esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error opening NVS handle: %s\n", esp_err_to_name(err));
            xtimer_sleep(DELAY_SECONDS);
            continue;
        }

        // Write the state to NVS (toggle between 0 and 1 for demonstration)
        static bool state = false;
        state = !state;
        int32_t state_value = state ? 1 : 0;
        err = nvs_set_i32(my_handle, STATE_KEY, state_value);

        // Commit the write
        if (err == ESP_OK) {
            err = nvs_commit(my_handle);
            if (err != ESP_OK) {
                printf("Error committing to NVS: %s\n", esp_err_to_name(err));
            }
        } else {
            printf("Error setting NVS value: %s\n", esp_err_to_name(err));
        }

        // Close the handle
        nvs_close(my_handle);

        // Wait for the next cycle
        xtimer_sleep(DELAY_SECONDS);
    }

    return 0;
}