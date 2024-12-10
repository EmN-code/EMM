
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <nvs.h>

#define NVS_NAMESPACE "device_state"
#define NVS_KEY "state"
#define NVS_VALUE_ON "1"
#define NVS_VALUE_OFF "0"

static nvs_handle_t nvs_handle;

void toggle_state(void)
{
    int32_t state;
    nvs_get_i32(nvs_handle, NVS_KEY, &state);

    if (state == 1) {
        state = 0;
    } else {
        state = 1;
    }

    nvs_set_i32(nvs_handle, NVS_KEY, state);
    nvs_commit(nvs_handle);

    printf("Device state updated to: %ld\n", (long int)state);
}

int main(void)
{
    int err;

    /* Initialize NVS */
    err = nvs_flash_init();
    if (err != ESP_OK) {
        printf("NVS initialization failed: %d\n", err);
        return 1;
    }

    /* Open NVS namespace */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        printf("NVS open failed: %d\n", err);
        return 1;
    }

    /* Set initial state if not set */
    int32_t state;
    err = nvs_get_i32(nvs_handle, NVS_KEY, &state);
    if (err != ESP_OK) {
        nvs_set_i32(nvs_handle, NVS_KEY, 1);
        nvs_commit(nvs_handle);
    }

    while (1) {
        toggle_state();
        xtimer_sleep(1);
    }

    /* Close NVS handle */
    nvs_close(nvs_handle);

    return 0;
}
