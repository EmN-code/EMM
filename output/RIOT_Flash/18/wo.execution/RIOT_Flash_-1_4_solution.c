#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "paho_mqtt.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "device_state"

void store_device_state(nvs_handle_t handle, int state) {
    nvs_set_i8(handle, STATE_KEY, state);
    nvs_commit(handle);
}

void app_main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle));

    int state = 0;
    while(1) {
        state = !state;
        store_device_state(my_handle, state);
        printf("Device state stored: %d\n", state);
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    nvs_close(my_handle);
}