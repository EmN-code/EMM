#include <stdio.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "thread_flags.h"
#include "thread.h"
#include "timer.h"
#include "xtimer.h"

#define DEVICE_STATE_KEY "device_state"

nvs_handle_t my_handle;
int device_state = 0;

void update_device_state(void) {
    // Toggle the state between 0 and 1
    device_state = !device_state;
    
    // Store the device state in NVS
    esp_err_t err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, device_state);
    if (err != ESP_OK) {
        printf("Error (%d) setting device state!\n", err);
    } else {
        nvs_commit(my_handle);
        printf("Device state updated to %d\n", device_state);
    }
}

void *timer_thread(void *arg) {
    (void)arg;
    while (1) {
        update_device_state();
        // Sleep for 1 second
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Open NVS handle for read/write
    ret = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (ret != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", ret);
    } else {
        // Initialize the thread
        thread_create(NULL, 0, PRIORITY_MAIN - 1, 0, timer_thread, NULL, "timer_thread");
    }

    // Wait indefinitely
    thread_sleep();
    nvs_close(my_handle);
    return 0;
}