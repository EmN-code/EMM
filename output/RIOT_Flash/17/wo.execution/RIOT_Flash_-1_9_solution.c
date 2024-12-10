#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "string.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include <unistd.h>

#define STORAGE_NAMESPACE "storage"

static char stack[THREAD_STACKSIZE_MAIN];

void *state_update_thread(void *arg) {
    (void)arg;
    nvs_handle_t nvs_handle;
    nvs_flash_init();
    nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvs_handle);

    int state = 0;
    while (1) {
        state = !state;
        nvs_set_i32(nvs_handle, "device_state", state);
        nvs_commit(nvs_handle);
        printf("State updated to: %d\n", state);
        xtimer_sleep(1);
    }

    nvs_close(nvs_handle);
    return NULL;
}

int main(void) {
    puts("NVS Storage Example");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  state_update_thread, NULL, "state_update_thread");
    return 0;
}