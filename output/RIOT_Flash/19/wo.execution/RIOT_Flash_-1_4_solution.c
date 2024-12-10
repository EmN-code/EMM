#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <MQTTClient.h>
#include <stdatomic.h>

#define STATE_KEY "device_state"
#define STATE_NAMESPACE "storage"
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE (KERNEL_CONF_STACKSIZE_DEFAULT)

void update_state(void *arg) {
    nvs_handle_t my_handle;
    esp_err_t err;
    int8_t state = 0;
    
    while (1) {
        /* Open NVS */
        err = nvs_open(STATE_NAMESPACE, NVS_READWRITE, &my_handle);
        if (err == ESP_OK) {
            /* Read state */
            err = nvs_get_i8(my_handle, STATE_KEY, &state);
            if (err == ESP_ERR_NVS_NOT_FOUND) {
                /* If not found, assume zero */
                state = 0;
            }
            
            /* Toggle state */
            state = !state;
            
            /* Write state */
            nvs_set_i8(my_handle, STATE_KEY, state);
            nvs_commit(my_handle);
            
            /* Close NVS */
            nvs_close(my_handle);
        }
        
        /* Wait for 1 second */
        xtimer_sleep(1);
    }
}

int main(void) {
    /* Initialize NVS */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }
    
    /* Create a thread to update state */
    char stack[THREAD_STACKSIZE];
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0, update_state, NULL, "update_state_thread");
    if (pid < 0) {
        printf("Thread creation failed\n");
        return 1;
    }
    
    return 0;
}
