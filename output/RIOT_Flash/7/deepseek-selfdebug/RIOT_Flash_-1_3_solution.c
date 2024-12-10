
#include "nvs.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "log.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"
#define TEMPERATURE_KEY "temperature"

static nvs_handle_t my_handle;

void update_device_state(int state) {
    int err = nvs_set_i32(my_handle, DEVICE_STATE_KEY, state);
    if (err != NVS_OK) {
        LOG_ERROR("Failed to set device state: %d\n", err);
    } else {
        err = nvs_commit(my_handle);
        if (err != NVS_OK) {
            LOG_ERROR("Failed to commit device state: %d\n", err);
        }
    }
}

int main(void) {
    int err;
    int device_state = 0;

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != NVS_OK) {
        LOG_ERROR("Failed to open NVS handle: %d\n", err);
        return 1;
    }

    while (1) {
        device_state = (device_state == 0) ? 1 : 0;
        update_device_state(device_state);

        xtimer_sleep(1);
    }

    nvs_close(my_handle);
    return 0;
}
