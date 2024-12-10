#include "thread_flags.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sys/socket.h"
#include "unistd.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "paho_mqtt.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "event/thread.h"

volatile atomic_int device_state = ATOMIC_VAR_INIT(0);

void update_device_state(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        return;
    }

    // Toggle state between 0 and 1
    int32_t state = atomic_load(&device_state);
    state = (state == 0) ? 1 : 0;
    atomic_store(&device_state, state);

    // Save the state in NVS
    nvs_set_i32(my_handle, "device_state", state);
    nvs_commit(my_handle);
    nvs_close(my_handle);
}

void* nvs_task(void *arg) {
    (void) arg; // Avoid unused parameter warning
    
    while (1) {
        update_device_state();
        usleep(1000000); // Sleep for 1 second
    }

    return NULL;
}

int main(void) {
    nvs_flash_init(); // Initialize NVS

    char stack[THREAD_STACKSIZE_MAIN];

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, nvs_task, NULL, "nvs_task");

    while (1) {
        // Primary application loop
    }

    return 0;
}