#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"

static void check_error_and_retry(nvs_handle_t handle, esp_err_t err)
{
    const int retry_delay_s = 1;
    while (err != ESP_OK) {
        printf("Error occurred: %s. Retrying...\n", esp_err_to_name(err));
        xtimer_sleep(retry_delay_s);
        // Retry operation
        if ((err = nvs_commit(handle)) == ESP_OK) {
            printf("Operation successful after retry.\n");
            break;
        }
    }
}

static int app_v1(void)
{
    esp_err_t err = nvs_flash_init();
    check_error_and_retry(NULL, err);
    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    check_error_and_retry(my_handle, err);

    uint8_t device_state = 0;
    while (1) {
        // Flip the device state
        device_state = 1 - device_state;

        // Store the state in NVS
        err = nvs_set_u8(my_handle, "device_state", device_state);
        check_error_and_retry(my_handle, err);

        // Commit the value
        err = nvs_commit(my_handle);
        check_error_and_retry(my_handle, err);

        printf("Device state %d stored in NVS.\n", device_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}

int main(void)
{
    return app_v1();
}
