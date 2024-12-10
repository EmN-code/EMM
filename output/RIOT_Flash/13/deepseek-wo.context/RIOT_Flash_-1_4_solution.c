
#include "xtimer.h"
#include "nvs.h"
#include "periph/gpio.h"
#include "fmt.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"
#define UPDATE_INTERVAL (1 * US_PER_SEC)

static nvs_handle_t nvs_handle;

void update_device_state(int32_t state) {
    int err;
    err = nvs_set_i32(nvs_handle, DEVICE_STATE_KEY, state);
    if (err != NVS_OK) {
        puts("Failed to write to NVS");
        return;
    }
    err = nvs_commit(nvs_handle);
    if (err != NVS_OK) {
        puts("Failed to commit to NVS");
    }
}

int main(void) {
    int err;
    int32_t state = 0;

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != NVS_OK) {
        puts("Failed to open NVS");
        return 1;
    }

    while (1) {
        state = (state == 0) ? 1 : 0;
        update_device_state(state);
        printf("Device state updated to: %" PRId32 "\n", state);
        xtimer_sleep(UPDATE_INTERVAL);
    }

    nvs_close(nvs_handle);
    return 0;
}
