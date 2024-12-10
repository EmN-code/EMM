#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>

int main(void) {
    nvs_handle_t my_handle;
    esp_err_t err;
    
    // Initialize the NVS
    err = nvs_flash_init();
    // Check for error
    while (err != ESP_OK) {
        printf("NVS flash init failed!\n");
        sleep(1); // Retry every second
        err = nvs_flash_init();
    }

    // Open
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    while (err != ESP_OK) {
        printf("NVS open failed!\n");
        sleep(1);
        err = nvs_open("storage", NVS_READWRITE, &my_handle);
    }

    int32_t temp = 0;
    while (1) {
        // Simulating temperature reading
        temp = rand() % 100;

        // Set temperature in NVS
        err = nvs_set_i32(my_handle, "temperature", temp);
        if (err != ESP_OK) {
            printf("Error (%d) writing temperature to NVS!\n", err);
            continue;
        }

        // Commit written value.
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("Error (%d) committing changes in NVS!\n", err);
            continue;
        }

        printf("Stored temperature: %ld\n", temp);

        sleep(1); // Delay for 1 second before next update
    }

    nvs_close(my_handle);

    return 0;
}