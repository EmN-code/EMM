#include <stdio.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <xtimer.h>

void update_device_state(nvs_handle_t handle) {
    uint8_t state;
    // Invert the state value (if 1 then set to 0, if 0 then set to 1)
    nvs_get_u8(handle, "device_state", &state);
    state = !state;
    nvs_set_u8(handle, "device_state", state);
    nvs_commit(handle);
    printf("Updated device state to: %d\n", state);
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Initialize NVS
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Error (%s) initializing NVS!\n", esp_err_to_name(err));
        return err;
    }
    
    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return err;
    }

    while (1) {
        update_device_state(my_handle);
        xtimer_sleep(1); // Sleep for 1 second
    }

    // Close NVS when done (not reachable in this loop)
    nvs_close(my_handle);
    return 0;
}