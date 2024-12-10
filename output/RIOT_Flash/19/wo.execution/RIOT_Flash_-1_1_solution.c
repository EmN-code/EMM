#include <thread.h>
#include <stdio.h>
#include <inttypes.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <nvs.h>

#define STORAGE_NAMESPACE "storage"

static char tag[] = "nvs_example";

static void store_device_state(int8_t state) {
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(tag, "Error (%d) opening NVS handle!\n", err);
    } else {
        err = nvs_set_i8(my_handle, "device_state", state);
        if (err != ESP_OK) {
            ESP_LOGE(tag, "Error (%d) writing state to NVS!\n", err);
        }

        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            ESP_LOGE(tag, "Error (%d) committing state in NVS!\n", err);
        }

        nvs_close(my_handle);
    }
}

static int8_t current_state = 0;

void *thread_func(void *arg) {
    while (1) {
        current_state = (current_state == 0) ? 1 : 0;
        store_device_state(current_state);
        ESP_LOGI(tag, "Stored device state: %d", current_state);
        /* Sleep for 1 second */
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    /* Create a new thread to handle the device state toggling and persistence */
    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_func, NULL, "nvs_thread");

    return 0;
}