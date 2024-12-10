#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <nvs_flash.h>
#include <nvs.h>

void write_to_nvs(nvs_handle_t *handle, int temp) {
    esp_err_t err;
    err = nvs_set_i32(*handle, "temperature", temp);
    if (err != ESP_OK) {
        printf("Failed to write temperature to NVS\n");
    } else {
        nvs_commit(*handle);
        printf("Temperature %d written to NVS\n", temp);
    }
}

int read_from_nvs(nvs_handle_t *handle) {
    int temp = 0;
    esp_err_t err;
    err = nvs_get_i32(*handle, "temperature", &temp);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("Temperature not found in NVS, initializing to 0\n");
        write_to_nvs(handle, 0);
    } else if (err != ESP_OK) {
        printf("Error reading temperature from NVS\n");
    } else {
        printf("Temperature %d read from NVS\n", temp);
    }
    return temp;
}

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("Failed to initialize NVS flash\n");
        return -1;
    }

    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Failed to open NVS handle\n");
        return -1;
    }

    int temperature = read_from_nvs(&my_handle);

    while (1) {
        temperature = (temperature == 1) ? 0 : 1;
        write_to_nvs(&my_handle, temperature);
        printf("Temperature updated to %d\n", temperature);
        xtimer_sleep(1);  // sleep for 1 second
    }

    nvs_close(my_handle);
    return 0;
}