#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <event/thread.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <thread.h>

#define STORAGE_NAMESPACE "storage"
#define STACK_SIZE       (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY  (THREAD_PRIORITY_MAIN - 1)

char stack[STACK_SIZE];

void store_device_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS flash
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error initializing NVS flash\n");
        return;
    }

    // Open NVS in read/write mode
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle\n");
        return;
    }

    // Store device state (toggle between 0 and 1)
    static bool state = false;
    state = !state;
    err = nvs_set_i32(my_handle, "device_state", state);
    if (err != ESP_OK) {
        printf("Error setting NVS value\n");
    }
    nvs_commit(my_handle);
    nvs_close(my_handle);
}

void *thread_func(void *arg) {
    (void)arg; // Unused
    while (1) {
        store_device_state();
        usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid;
    thread_pid = thread_create(stack, STACK_SIZE, THREAD_PRIORITY, THREAD_CREATE_STACKTEST, thread_func, NULL, "nvs_thread");
    thread_join(thread_pid);
    return 0;
}