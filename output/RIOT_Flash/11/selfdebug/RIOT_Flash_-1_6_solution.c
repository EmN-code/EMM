# Include necessary header files
#include <nvs_flash.h>
#include <nvs.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>

#define STORAGE_NAMESPACE "storage"

static void store_device_state(nvs_handle_t *my_handle, const char *key, int value) {
    // Write
    esp_err_t err = nvs_set_i32(*my_handle, key, value);
    if (err != ESP_OK) {
        printf("Error (%s) writing!
", esp_err_to_name(err));
    } else {
        printf("Device state updated to: %d\n", value);
    }
}

static int read_device_state(nvs_handle_t *my_handle, const char *key) {
    int value = 0;
    // Read
    esp_err_t err = nvs_get_i32(*my_handle, key, &value);
    switch (err) {
        case ESP_OK:
            printf("Device state = %d\n", value);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The state is not initialized yet!");
            break;
        default :
            printf("Error (%s) reading!
", esp_err_to_name(err));
    }
    return value;
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated
      // and needs to be erased
      ESP_ERROR_CHECK(nvs_flash_erase());
      err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    nvs_handle_t my_handle;
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!
", esp_err_to_name(err));
        return err;
    } else {
        printf("NVS handle opened successfully!\n");
    }

    const char *state_key = "device_state";
    int current_state = read_device_state(&my_handle, state_key);

    while (1) {
        // Toggle state
        current_state = !current_state;
        store_device_state(&my_handle, state_key, current_state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    // Close NVS
    nvs_close(my_handle);
    return 0;
}