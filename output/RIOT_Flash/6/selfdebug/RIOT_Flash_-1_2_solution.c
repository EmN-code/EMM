#include "nvs_flash.h"
#include "nvs.h"
#include "thread.h"
#include "stdio.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

static char stack[STACKSIZE];

// Function to simulate reading temperature
int read_temperature() {
    return 25; // Placeholder temperature value
}

void store_temperature(void) {
    nvs_handle_t my_handle;
    esp_err_t err1 = nvs_flash_init();
    if (err1 == ESP_ERR_NVS_NO_FREE_PAGES || err1 == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err1 = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err1);

    err1 = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err1 != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err1));
    } else {
        int temp = read_temperature();

        err1 = nvs_set_i32(my_handle, "temperature", temp);
        if (err1 != ESP_OK) {
            printf("Error (%s) saving temperature to NVS!\n", esp_err_to_name(err1));
        }

        // Commit changes
        err1 = nvs_commit(my_handle);
        if (err1 != ESP_OK) {
            printf("Error (%s) committing updates in NVS!\n", esp_err_to_name(err1));
        }

        nvs_close(my_handle);
        printf("Temperature %d stored in NVS.\n", temp);
    }
}

void *temperature_thread(void *arg) {
    (void)arg;
    while (1) {
        store_temperature();
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    printf("Starting temperature storage thread\n");
    kernel_pid_t temp_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, temperature_thread, NULL, "temperature_thread");
    if (temp_thread_pid != THREAD_PID_UNDEF) {
        printf("Temperature storage thread started\n");
    } else {
        printf("Failed to create temperature storage thread\n");
    }
    return 0;
}