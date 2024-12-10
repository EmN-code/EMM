#include <ztimer.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <event/thread.h>
#include <thread.h>
#include <stdlib.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <mqttclient.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <unistd.h>

#define STORAGE_NAMESPACE "storage"
#define DELAY_INTERVAL (1U * 1000000U) // 1 second in microseconds

static kernel_pid_t pid;
static char thread_stack[THREAD_STACKSIZE_MAIN];

void *temperature_task(void *arg) {
    (void)arg;
    nvs_handle_t my_handle;
    int32_t state = 0;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS: %d\n", err);
        return NULL;
    }

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle: %d\n", err);
        return NULL;
    }

    while (1) {
        // Toggle the state
        state = (state == 0) ? 1 : 0;

        // Write state to NVS
        err = nvs_set_i32(my_handle, "state", state);
        if (err != ESP_OK) {
            printf("Error writing to NVS: %d\n", err);
        }

        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error committing to NVS: %d\n", err);
        }

        // Print current state
        printf("Current state: %d\n", state);

        // Wait for 1 second
        ztimer_sleep(ZTIMER_MSEC, DELAY_INTERVAL / 1000);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return NULL;
}

int main(void) {
    // Create a thread to run the temperature_task
    pid = thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 
                        THREAD_CREATE_STACKTEST, temperature_task, NULL, "temperature_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }

    ztimer_sleep(ZTIMER_MSEC, DELAY_INTERVAL / 1000); // Let the main thread sleep to keep the program running
    return 0;
}