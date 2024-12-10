#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <timex.h>
#include <unistd.h>
#include <xtimer.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <mqttclient.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <esp_wifi.h>
#include <esp_system.h>

#define STORAGE_NAMESPACE "storage"

static void *nvs_task(void *arg) {
    esp_err_t err;
    nvs_handle_t my_handle;
    int32_t device_state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    while (1) {
        // Open NVS handle
        err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err == ESP_OK) {
            // Write state value to NVS
            err = nvs_set_i32(my_handle, "device_state", device_state);
            if (err == ESP_OK) {
                // Commit the value
                err = nvs_commit(my_handle);
                if (err == ESP_OK) {
                    printf("Device state saved: %ld\n", (long)device_state);
                }
                else {
                    printf("Failed to commit device state: %d\n", err);
                }
            }
            else {
                printf("Failed to set device state: %d\n", err);
            }
            // Close NVS handle
            nvs_close(my_handle);
        }
        else {
            printf("Error (%d) opening NVS handle!\n", err);
        }

        // Toggle state between 0 and 1
        device_state = !device_state;

        // Sleep for 1 second
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, nvs_task, NULL, "nvs_task");
    return 0;
}