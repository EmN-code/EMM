
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

int main(void)
{
    nvs_handle_t my_handle;
    esp_err_t err;
    int32_t state = 0;

    /* Initialize NVS */
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS Flash Init failed: %d\n", err);
        return 1;
    }

    /* Open NVS namespace */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("NVS Open failed: %d\n", err);
        return 1;
    }

    while (1) {
        /* Toggle state */
        state = (state == 0) ? 1 : 0;

        /* Write state to NVS */
        err = nvs_set_i32(my_handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("NVS Set failed: %d\n", err);
            return 1;
        }

        /* Commit writes */
        err = nvs_commit(my_handle);
        if (err != ESP_OK) {
            printf("NVS Commit failed: %d\n", err);
            return 1;
        }

        /* Read state from NVS */
        err = nvs_get_i32(my_handle, NVS_KEY, &state);
        if (err != ESP_OK) {
            printf("NVS Get failed: %d\n", err);
            return 1;
        }

        /* Print the current state */
        printf("Current state: %" PRId32 "\n", state);

        /* Wait for 1 second */
        xtimer_sleep(1);
    }

    /* Close NVS handle */
    nvs_close(my_handle);

    return 0;
}
