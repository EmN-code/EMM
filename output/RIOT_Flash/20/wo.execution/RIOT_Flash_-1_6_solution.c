#include "xtimer.h"
#include "ztimer.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mbox.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "unistd.h"

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5

static char stack[THREAD_STACKSIZE_MAIN];
int state = 0;
nvs_handle_t my_handle;

void *nvs_write_thread(void *arg) {
    (void)arg;
    while (1) {
        nvs_set_i32(my_handle, "device_state", state);
        nvs_commit(my_handle);
        state = (state == 0) ? 1 : 0;  // Toggle the state
        xtimer_sleep(1);  // Sleep for a second using xtimer
    }
    return NULL;
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    // Create a thread to continuously write to NVS
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, nvs_write_thread, NULL, "nvs_write_thread");

    // Main thread can perform other tasks
    while (1) {
        // Placeholder for other code
        xtimer_sleep(10);
    }

    // Close NVS handle (although we won't reach here)
    nvs_close(my_handle);
    return 0;
}