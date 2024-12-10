#include <xtimer.h> // For xtimer_sleep
#include <stdio.h> // For printf
#include <nvs_flash.h> // For NVS flash operations
#include <esp_err.h> // For ESP error codes

#define STORAGE_NAMESPACE "storage"
#define TEMP_KEY "temperature"

void write_temperature_to_nvs(int temperature) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // Write
    err = nvs_set_i32(my_handle, TEMP_KEY, temperature);
    if (err != ESP_OK) {
        printf("Failed to write to NVS (%s)!\n", esp_err_to_name(err));
    }

    // Commit
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit changes to NVS (%s)!\n", esp_err_to_name(err));
    }

    // Close
    nvs_close(my_handle);
}

int generate_temperature() {
    // Simulate reading a temperature sensor
    // You would replace this with real sensor reading code
    return random_uint32_range(0, 35); // Random temperature between 0 and 35
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    while (1) {
        int temperature = generate_temperature();
        printf("New Temperature: %d\n", temperature);
        write_temperature_to_nvs(temperature);
        xtimer_sleep(1); // Sleep for 1 second
    }
    return 0;
}