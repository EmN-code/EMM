#include <nvs_flash.hpp>
#include <stdio.h>
#include "xtimer.h"

#define STORAGE_NAMESPACE "storage"

void init_nvs()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void store_state(int state)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));

    // Write state to NVS
    err = nvs_set_i32(my_handle, "state", state);
    if (err != ESP_OK) printf("Error (%s) setting value!\n", esp_err_to_name(err));

    // Commit written value.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) printf("Error (%s) committing value!\n", esp_err_to_name(err));

    // Close
    nvs_close(my_handle);
}

int load_state()
{
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 0; // default state

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err == ESP_OK) {
        // Read
        err = nvs_get_i32(my_handle, "state", &state);

        switch (err) {
            case ESP_OK:
                printf("State found: %d\n", state);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                printf("The state is not initialized yet!\n");
                break;
            default :
                printf("Error (%s) reading state!\n", esp_err_to_name(err));
        }
        nvs_close(my_handle);
    } else {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    return state;
}

int main(void)
{
    init_nvs();

    int state = load_state();  // Load the existing state

    while (1) {
        // Toggle state
        state = !state;

        // Store the new state
        store_state(state);

        // Wait for 1 second
        xtimer_sleep(1);
    }

    return 0;
}