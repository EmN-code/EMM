#include "stdio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "kernel_defines.h"
#include "thread.h"

// Function to initialize NVS
void init_nvs(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

// Function to store device state in NVS
void store_device_state(int state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }
    
    err = nvs_set_i32(my_handle, "state", state);
    if (err != ESP_OK) {
        printf("Failed to write state to NVS!\n");
    }
    
    err = nvs_commit(my_handle);
    if (err != ESP_OK) {
        printf("Failed to commit state to NVS!\n");
    }
    nvs_close(my_handle);
}

int main(void) {
    init_nvs();
    
    while (1) {
        static int state = 0; // Toggle between 0 and 1
        store_device_state(state);
        state = !state;
        
        printf("Stored state: %d\n", state);

        // Sleep for 1 second
        usleep(1000000);
    }
    return 0;
}