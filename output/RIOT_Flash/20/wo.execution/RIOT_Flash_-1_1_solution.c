#include <thread.h>
#include <timer.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"

static nvs_handle_t my_handle;
static int device_state = 0;

void toggle_device_state(void) {
    // Toggle state
    device_state = !device_state;

    // Write state back to NVS
    nvs_set_i32(my_handle, "device_state", device_state);
    nvs_commit(my_handle);

    printf("Device state updated to: %d\n", device_state);
}

void *timer_thread(void *arg) {
    (void)arg;
    while(1) {
        toggle_device_state();
        usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            // NVS partition was truncated and needs to be erased
            ESP_ERROR_CHECK(nvs_flash_erase());
            err = nvs_flash_init();
        }
    }
    ESP_ERROR_CHECK(err);

    // Open NVS
    ESP_ERROR_CHECK(nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle));

    // Read device state from NVS
    int32_t stored_state = 0;
    err = nvs_get_i32(my_handle, "device_state", &stored_state);
    if (err == ESP_OK) {
        device_state = stored_state;
        printf("Device state restored to: %d\n", device_state);
    } else {
        printf("Failed to read state from NVS, setting default: 0\n");
    }

    // Start thread to update the device state every second
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, timer_thread, NULL, "timer_thread");

    return 0;
}