#include <xtimer.h>
#include <random.h>
#include <nvs.h>
#include <inttypes.h>
#include <stdio.h>

nvs_handle_t my_handle;

int main(void) {
    int32_t device_state = 0;
    esp_err_t err;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        printf("Open NVS handle successfully.\n");
    }

    printf("Reading initial value...\n");
    err = nvs_get_i32(my_handle, "device_state", &device_state);
    switch (err) {
        case ESP_OK:
            printf("Device state read successfully: %" PRId32 "\n", device_state);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            device_state = 0;
            break;
        default :
            printf("Error (%d) reading the value from NVS!\n", err);
            break;
    }

    while (1) {
        device_state = (device_state == 0) ? 1 : 0;
        printf("Writing device state %" PRId32 " to NVS...\n", device_state);
        err = nvs_set_i32(my_handle, "device_state", device_state);
        if (err != ESP_OK) {
            printf("Failed to write to NVS!\n");
        } else {
            printf("Successfully written to NVS!\n");
        }
        nvs_commit(my_handle);

        int32_t temperature = (int32_t)random_uint32_range(0, 100);
        printf("Temperature: %" PRId32 "\n", temperature);
        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}