
#include "xtimer.h"
#include "nvs.h"
#include "periph/gpio.h"
#include "log.h"

#define NVS_NAMESPACE "storage"
#define NVS_KEY "device_state"
#define DELAY_MS (1000U)

static nvs_handle_t nvs_handle;

void update_device_state(int state) {
    int err;
    err = nvs_set_i32(nvs_handle, NVS_KEY, state);
    if (err != NVS_OK) {
        LOG_ERROR("Failed to set NVS value: %d\n", err);
        return;
    }
    err = nvs_commit(nvs_handle);
    if (err != NVS_OK) {
        LOG_ERROR("Failed to commit NVS value: %d\n", err);
    }
}

int main(void) {
    int err;
    int state = 0;

    /* Initialize NVS */
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != NVS_OK) {
        LOG_ERROR("Failed to open NVS handle: %d\n", err);
        return 1;
    }

    while (1) {
        /* Toggle state */
        state = (state == 0) ? 1 : 0;

        /* Update state in NVS */
        update_device_state(state);

        /* Wait for 1 second */
        xtimer_sleep(DELAY_MS / 1000);
    }

    /* Close NVS handle */
    nvs_close(nvs_handle);

    return 0;
}
