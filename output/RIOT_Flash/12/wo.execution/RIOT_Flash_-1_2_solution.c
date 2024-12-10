#include <sys/socket.h>
#include <thread_flags.h>
#include <thread.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_wifi.h>
#include <event/thread.h>
#include <esp_system.h>

#define STORAGE_NAMESPACE "storage"
#define STATE_KEY "state"

// Function prototypes
void update_state(void *arg);

// Function to initialize NVS
void init_nvs(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

// Function to update the state in NVS
void update_state(void *arg) {
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    ESP_ERROR_CHECK(err);

    int16_t state = 0;
    while (1) {
        state = !state; // Toggle state between 0 and 1
        err = nvs_set_i16(my_handle, STATE_KEY, state);
        ESP_ERROR_CHECK(err);

        err = nvs_commit(my_handle);
        ESP_ERROR_CHECK(err);

        vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait for 1 second
    }

    nvs_close(my_handle);
}

int main(void) {
    // Initialize NVS
    init_nvs();

    // Start the update state task
    xTaskCreate(&update_state, "update_state", 2048, NULL, 5, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // Loop forever
    while (1) {
        // Sleep thread to prevent busy looping
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return 0;
}