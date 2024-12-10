#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"
#define TIMER_INTERVAL (1U * 1000000U)  // 1 second in microseconds

void update_device_state(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    int8_t state = 1;  // Example device state

    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return;
    }

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE,&my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write
    err = nvs_set_i32(my_handle, "state", state);
    if (err != ESP_OK) {
        printf("Error (%s) writing to NVS!\n", esp_err_to_name(err));
    }

    // Commit written value
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) committing NVS!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(my_handle);
}

int main(void)
{
    while (1) {
        update_device_state();
        xtimer_sleep(1);  // Sleep for 1 second
    }
    return 0;
}