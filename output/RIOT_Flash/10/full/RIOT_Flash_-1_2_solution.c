#include "nvs_flash.h"
#include "nvs.h"
#include "thread.h"
#include "event/thread.h"

#define STORAGE_NAMESPACE "storage"

void *store_device_state(void *arg) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%d) initializing NVS\n", err);
        return NULL;
    }

    // Open NVS handle
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle\n", err);
        return NULL;
    }

    // Initialize the stored state
    int32_t state = 0;

    while (1) {
        // Set value of the device state in NVS
        err = nvs_set_i32(my_handle, "device_state", state);
        if (err != ESP_OK) {
            printf("Error (%d) setting device state in NVS\n", err);
        } else {
            printf("Device state set to %d\n", state);
            nvs_commit(my_handle);
        }

        // Toggle state between 0 and 1
        state = !state;

        // Sleep for a second
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    // Close NVS handle (never reached)
    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, store_device_state, NULL, "device_state");
    
    return 0;
}