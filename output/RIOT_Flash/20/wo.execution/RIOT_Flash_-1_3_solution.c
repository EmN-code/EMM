#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <event/thread.h>

#define STACKSIZE 1024
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

atomic_bool device_state;

void *update_storage(void *arg) {
    nvs_handle_t my_handle;
    ESP_ERROR_CHECK(nvs_open("storage", NVS_READWRITE, &my_handle));
    
    while (1) {
        int state = atomic_load(&device_state);
        ESP_ERROR_CHECK(nvs_set_i32(my_handle, "device_state", state));
        ESP_ERROR_CHECK(nvs_commit(my_handle));
        
        usleep(1000000); // Sleep for 1 second
    }
    
    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    
    char stack[STACKSIZE];
    thread_create(stack, sizeof(stack), PRIORITY, CREATE_STACKTEST, update_storage, NULL, "UpdateStorageThread");
    
    while (1) {
        atomic_store(&device_state, !atomic_load(&device_state));
        usleep(1000000); // Sleep for 1 second
    }
    
    return 0;
}