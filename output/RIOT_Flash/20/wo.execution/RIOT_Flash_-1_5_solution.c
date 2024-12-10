#include <unistd.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <stdlib.h>

nvs_handle_t my_handle;

void store_temperature(int temperature) {
    // Store the temperature in NVS
    nvs_set_i32(my_handle, "temperature", temperature);
    nvs_commit(my_handle);
}

void *nvs_task(void *arg) {
    while (1) {
        int temperature = rand() % 100; // Simulated temperature value
        store_temperature(temperature);
        printf("Stored temperature: %d\n", temperature);
        sleep(1); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    // Create a thread to continuously store temperature
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, nvs_task, NULL, "nvs_task");

    // The main function can perform other tasks or go idle
    return 0;
}