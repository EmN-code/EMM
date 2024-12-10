#include "thread.h"
#include "msg.h"
#include "nvs_flash.h"
#include "nvs.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define TEMP_SENSOR_DATA 1 // Example temperature data, replace with actual sensor reading

static unsigned char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *nvs_storage_thread(void *arg) {
    nvs_handle_t handle;
    esp_err_t err;

    msg_init_queue(msg_queue, 8);

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return NULL;
    }

    // Open NVS handle
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return NULL;
    }

    // Continuously write state to NVS every second
    while (1) {
        // Write data to NVS
        err = nvs_set_i8(handle, NVS_KEY, TEMP_SENSOR_DATA);
        if (err == ESP_OK) {
            printf("Stored value: %d\n", TEMP_SENSOR_DATA);

            // Commit written value.
            err = nvs_commit(handle);
            if (err != ESP_OK) {
                printf("Error (%s) committing to NVS!\n", esp_err_to_name(err));
            }
        } else {
            printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
        }

        // Wait for one second
        thread_sleep();
    }

    // Close NVS handle
    nvs_close(handle);

    return NULL;
}

int main(void) {
    // Create and start the thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  nvs_storage_thread, NULL, "nvs_storage");
    
    return 0;
}
