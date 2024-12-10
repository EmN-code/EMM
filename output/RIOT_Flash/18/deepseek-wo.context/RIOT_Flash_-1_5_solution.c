
#include "nvs.h"
#include "thread.h"
#include "xtimer.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"

static void update_state(void)
{
    nvs_handle_t handle;
    int32_t state = 1; // Example state value
    esp_err_t err;

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        printf("Error (%d) opening NVS handle!\n", err);
    } else {
        // Write state to NVS
        err = nvs_set_i32(handle, NVS_KEY, state);
        if (err != ESP_OK) {
            printf("Error (%d) writing to NVS!\n", err);
        } else {
            err = nvs_commit(handle);
            if (err != ESP_OK) {
                printf("Error (%d) committing to NVS!\n", err);
            }
        }
        nvs_close(handle);
    }
}

int main(void)
{
    while (1) {
        update_state();
        xtimer_sleep(1); // Sleep for 1 second
    }
    return 0;
}
