#include <stdio.h>
#include <xtimer.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <string.h>

#define STORAGE_NAMESPACE "storage"

static const char *TAG = "nvs_example";

void update_device_state(nvs_handle_t handle) {
    int rc;
    uint8_t state = 0;
    size_t length = sizeof(state);

    // Attempt to read the previously stored state
    rc = nvs_get_u8(handle, "device_state", &state);
    if (rc != ESP_OK) {
        if (rc == ESP_ERR_NVS_NOT_FOUND) {
            printf("State not found, initializing to 0\n");
            state = 0;
        } else {
            printf("Error reading NVS\n");
            return;
        }
    }

    // Toggle state
    state = (state == 1) ? 0 : 1;
    printf("Updating state to: %d\n", state);

    // Write the state back to NVS
    rc = nvs_set_u8(handle, "device_state", state);
    if (rc != ESP_OK) {
        printf("Error writing NVS\n");
        return;
    }
    nvs_commit(handle);
}

int main(void) {
    nvs_handle_t handle;
    esp_err_t rc;

    printf("Initializing NVS\n");

    rc = nvs_flash_init();
    if (rc == ESP_ERR_NVS_NO_FREE_PAGES || rc == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("Erasing storage and retrying NVS init\n");
        ESP_ERROR_CHECK(nvs_flash_erase());
        rc = nvs_flash_init();
    }
    ESP_ERROR_CHECK(rc);

    printf("Opening NVS handle\n");

    while (1) {
        rc = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
        if (rc != ESP_OK) {
            printf("Error opening NVS handle, retrying\n");
            xtimer_sleep(1);
            continue;
        } else {
            break;
        }
    }

    while (1) {
        update_device_state(handle);
        xtimer_sleep(1);
    }

    nvs_close(handle);
    return 0;
}
