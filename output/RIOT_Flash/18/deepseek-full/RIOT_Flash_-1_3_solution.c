
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
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
static atomic_int device_state = 0;

void toggle_state(void)
{
    device_state = !device_state;
    int32_t state = device_state;
    nvs_set_i32(my_handle, DEVICE_STATE_KEY, state);
    nvs_commit(my_handle);
    printf("Device state toggled to: %d\n", state);
}

int main(void)
{
    int err;

    /* Initialize NVS */
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        printf("NVS initialization failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    /* Open NVS */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %s\n", esp_err_to_name(err));
        return -1;
    }

    /* Main loop */
    while (1) {
        toggle_state();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    /* Close NVS */
    nvs_close(my_handle);

    return 0;
}
