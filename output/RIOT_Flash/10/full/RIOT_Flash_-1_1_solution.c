#include "xtimer.h"
#include "thread.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "stdio.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];

void *run_temperature_monitoring(void *arg) {
    (void)arg;  // Unused parameter
    
    nvs_flash_init();
    nvs_handle_t my_handle;

    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
        return NULL;
    }

    int16_t temperature = 0;
    size_t required_size = sizeof(temperature);

    // Load the initial state
    err = nvs_get_i16(my_handle, "temperature", &temperature);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
        printf("Error (%d) reading temperature from NVS\n", err);
    }

    while (1) {
        // Simulate temperature measurement
        temperature = (temperature == 0) ? 1 : 0;  // Toggle between 0 and 1

        // Write the new state to NVS
        err = nvs_set_i16(my_handle, "temperature", temperature);
        if (err == ESP_OK) {
            nvs_commit(my_handle);
            printf("Temperature updated in NVS: %d\n", temperature);
        } else {
            printf("Failed to update temperature in NVS: %d\n", err);
        }

        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    nvs_flash_init();
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, run_temperature_monitoring, NULL, "temp_monitoring");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}