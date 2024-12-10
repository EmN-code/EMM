#include <stdio.h>
#include <unistd.h> // for sleep function
#include <time.h>
#include "nvs_flash.h" // Include NVS functions
#include "nvs.h" // For nvs_handle

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    nvs_handle_t my_handle;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error opening NVS handle!");
        return err;
    }

    int32_t device_state = 1;
    while (1) {
        err = nvs_set_i32(my_handle, "device_state", device_state);
        if (err != ESP_OK) printf("Failed to set device_state!");

        err = nvs_commit(my_handle);
        if (err != ESP_OK) printf("Failed to commit!");

        printf("Device state %d is stored at %ld.\n", device_state, time(NULL));

        // Toggle state between 0 and 1
        device_state = device_state == 0 ? 1 : 0;

        sleep(1); // Sleep for 1 second
    }

    nvs_close(my_handle);
    return 0;
}