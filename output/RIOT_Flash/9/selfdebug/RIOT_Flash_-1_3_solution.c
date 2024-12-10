#include <stdio.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <thread.h>
#include <ztimer.h>

#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define SLEEP_DURATION (1000000U) // 1 second in microseconds

static char thread_stack[STACK_SIZE];

void *nvs_thread(void *arg) {
    (void)arg;
    nvs_handle_t nvs_handle;
    esp_err_t err;
    uint8_t state = 0;

    // Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    while (1) {
        // Open NVS handle for read/write
        err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        } else {
            // Read the value
            err = nvs_get_u8(nvs_handle, "state", &state);
            switch (err) {
                case ESP_OK:
                    printf("State = %d\n", state);
                    break;
                case ESP_ERR_NVS_NOT_FOUND:
                    printf("The value is not initialized yet!\n");
                    break;
                default :
                    printf("Error (%s) reading!\n", esp_err_to_name(err));
            }

            // Toggle and write the value back
            state = !state;
            err = nvs_set_u8(nvs_handle, "state", state);
            if (err != ESP_OK) {
                printf("Failed to write NVS state!\n");
            }

            // Commit written value.
            err = nvs_commit(nvs_handle);
            if (err != ESP_OK) {
                printf("Failed to commit NVS state!\n");
            }

            // Close NVS handle
            nvs_close(nvs_handle);
        }

        // Sleep for 1 second
        ztimer_sleep(ZTIMER_USEC, SLEEP_DURATION);
    }

    return NULL;
}

int main(void) {
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, nvs_thread, NULL, "nvs_thread");
    
    return 0;
}