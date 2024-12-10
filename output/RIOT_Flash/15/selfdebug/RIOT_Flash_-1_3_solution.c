#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"
#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds

kernel_pid_t main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased.
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    uint32_t device_state = 0;
    uint32_t current_state;
    size_t required_size = sizeof(current_state);

    while (1) {
        // Read the state from NVS
        err = nvs_get_u32(my_handle, "device_state", &current_state);
        switch (err) {
            case ESP_OK:
                printf("State read from NVS: %d\n", current_state);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The state is not initialized yet!");
                break;
            default :
                printf("Error (%s) reading!");
        }

        // Toggle the state
        device_state = (current_state == 1) ? 0 : 1;
        printf("State to be written: %d\n", device_state);

        // Write the new state to NVS
        err = nvs_set_u32(my_handle, "device_state", device_state);
        ESP_ERROR_CHECK(err);

        // Commit the written value to NVS
        err = nvs_commit(my_handle);
        ESP_ERROR_CHECK(err);

        // Delay for 1 second
        xtimer_sleep(1);
    }

    // Close NVS handle
    nvs_close(my_handle);

    return 0;
}