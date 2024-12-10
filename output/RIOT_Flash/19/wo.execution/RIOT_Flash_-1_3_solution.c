#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <kernel_defines.h>
#include <stdatomic.h>
#include <event/thread.h>
#include <MQTTClient.h>

#define STACKSIZE       (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

nvs_handle_t my_handle;

void *nvs_write_thread(void *arg) {
    while(1) {
        int err;

        // Open
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) opening NVS handle!\n", err);
        } else {
            // Read temperature variable, assumed from some sensor read function (simulate here)
            int temperature;

            // Simulate a sensor read (a random temperature value for the sake of demonstration)
            temperature = rand() % 100;

            // Write the temperature to NVS
            err = nvs_set_i8(my_handle, "device_state", (int8_t)temperature);
            if (err != ESP_OK) {
                printf("Failed to write to NVS!\n");
            } else {
                nvs_commit(my_handle);
            }
            // Close
            nvs_close(my_handle);
        }

        // Wait for 1 second
        usleep(1000000);
    }
    return NULL;
}

int main(void) {
    puts("RIOT NVS storage example
");

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated and needs to be erased
      ret = nvs_flash_erase();
      if (ret == ESP_OK) {
        ret = nvs_flash_init();
      }
    }
    printf((ret != ESP_OK) ? "Failed to init NVS\n" : "NVS initialized\n");

    char thread_stack[STACKSIZE];
    kernel_pid_t thread_pid = thread_create(thread_stack, STACKSIZE,
                                            THREAD_PRIORITY, 0,
                                            nvs_write_thread, NULL, "nvs_writer");

    /* This is typically a loop in RIOT background tasks */
    while(1) {
        usleep(1000000);
    }

    return 0;
}