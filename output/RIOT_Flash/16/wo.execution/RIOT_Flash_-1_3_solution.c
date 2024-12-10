#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define STORAGE_NAMESPACE "storage"

void store_device_state(nvs_handle_t handle, int state) {
    nvs_set_i32(handle, "device_state", state);
    nvs_commit(handle);
}

void *thread_function(void *arg) {
    (void)arg;
    nvs_handle_t my_handle;
    int state = 0;

    nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    while (1) {
        state = state == 0 ? 1 : 0;
        store_device_state(my_handle, state);
        xtimer_sleep(1);
    }
    nvs_close(my_handle);
    return NULL;
}

int main(void) {
    nvs_flash_init();

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  thread_function, NULL, "device_state_thread");

    return 0;
}
