#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kernel_defines.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <event/thread.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Function to emulate temperature update
int get_temperature(void) {
    return 25;  // Dummy temperature value
}

// Function to emulate device state update
bool get_device_state(void) {
    return true;  // Dummy device state
}

// Task function to store device state in NVS
void *temperature_update_task(void *arg) {
    nvs_handle_t my_handle;
    nvs_open("storage", NVS_READWRITE, &my_handle);
    while (1) {
        int temp = get_temperature();
        bool state = get_device_state();

        // Update state and temperature in storage
        nvs_set_i32(my_handle, "temperature", temp);
        nvs_set_i32(my_handle, "state", state ? 1 : 0);

        // Commit changes to NVS
        nvs_commit(my_handle);

        // Sleep for 1 second
        sleep(1);
    }
    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    // Initialize NVS
    nvs_flash_init();

    char stack[THREAD_STACKSIZE_MAIN];

    // Create a thread for the temperature update task
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, temperature_update_task, NULL, "temperature_update_thread");

    return 0;
}