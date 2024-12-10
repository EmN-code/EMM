
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <time_units.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <ztimer.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <xtimer.h>

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static nvs_handle_t nvs_handle;
static atomic_bool device_state = ATOMIC_VAR_INIT(false);

void update_state(void)
{
    atomic_store(&device_state, !atomic_load(&device_state));
}

void store_state(void)
{
    uint8_t state = atomic_load(&device_state) ? 1 : 0;
    nvs_set_u8(nvs_handle, DEVICE_STATE_KEY, state);
    nvs_commit(nvs_handle);
}

void load_state(void)
{
    uint8_t state;
    if (nvs_get_u8(nvs_handle, DEVICE_STATE_KEY, &state) == ESP_OK) {
        atomic_store(&device_state, state == 1);
    }
}

int main(void)
{
    if (nvs_flash_init() != ESP_OK) {
        printf("NVS Flash initialization failed\n");
        return 1;
    }

    if (nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle) != ESP_OK) {
        printf("NVS handle open failed\n");
        return 1;
    }

    load_state();

    while (1) {
        update_state();
        store_state();
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return 0;
}
