#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

void app_main() {
    if (nvs_flash_init() != ESP_OK) {
        printf("NVS Flash init failed!\n");
        return;
    }

    nvs_handle_t my_handle;
    if (nvs_open("storage", NVS_READWRITE, &my_handle) != ESP_OK) {
        printf("Error opening NVS handle!\n");
        return;
    }

    int8_t device_state = 0;
    while (true) {
        device_state = !device_state; // Toggle state
        if (nvs_set_i8(my_handle, "device_state", device_state) != ESP_OK) {
            printf("Failed to write to NVS!\n");
        }
        if (nvs_commit(my_handle) != ESP_OK) {
            printf("Failed to commit changes to NVS!\n");
        } else {
            printf("Device state saved: %d\n", device_state);
        }
        for (uint32_t i = 0; i < 100000000; ++i) {
            ; // Simulate sleep
        }
    }

    nvs_close(my_handle); // Should close only if there's a loop break or exit
}

int main() {
    app_main();
    return 0;
}