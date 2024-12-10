#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <timex.h>
#include <time.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"

void store_temperature(nvs_handle_t handle) {
    int32_t temperature = rand() % 100; // Simulate temperature readings
    nvs_set_i32(handle, "temperature", temperature);
    nvs_commit(handle);
}

void *temperature_task(void *arg) {
    nvs_handle_t my_handle;
    nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    while(1) {
        store_temperature(my_handle);
        usleep(1000000); // Sleep for 1 second
    }
    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    nvs_flash_init();
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, temperature_task, NULL, "temperature");
    pause();
    return 0;
}