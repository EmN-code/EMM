#include <time_units.h>
#include <thread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <esp_system.h>
#include <nvs.h>
#include <nvs_flash.h>

void toggle_device_state(int *current_state) {
    *current_state = !(*current_state); // Toggle between 0 and 1
}

int read_device_state(nvs_handle_t handle) {
    int32_t state = 0;
    nvs_get_i32(handle, "device_state", &state);
    return (int)state;
}

void write_device_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, "device_state", state);
    nvs_commit(handle);
}

int main(void) {
    nvs_flash_init();
    nvs_handle_t handle;
    nvs_open("storage", NVS_READWRITE, &handle);

    int current_state = read_device_state(handle);

    while (1) {
        toggle_device_state(&current_state);
        write_device_state(handle, current_state);
        printf("Device state: %d\n", current_state);
        sleep(1); // Sleep for 1 second
    }

    nvs_close(handle);
    return 0;
}