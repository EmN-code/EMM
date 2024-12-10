
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <mutex.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <ztimer.h>

#define NVS_PARTITION_NAME "storage"
#define NVS_KEY "device_state"

static mutex_t nvs_mutex = MUTEX_INIT;

void store_state(uint8_t state) {
    mutex_lock(&nvs_mutex);
    // Simulate NVS write operation
    printf("Storing state: %d\n", state);
    mutex_unlock(&nvs_mutex);
}

uint8_t read_state(void) {
    mutex_lock(&nvs_mutex);
    // Simulate NVS read operation
    uint8_t state = 0; // Default state
    printf("Reading state: %d\n", state);
    mutex_unlock(&nvs_mutex);
    return state;
}

int main(void) {
    puts("RIOT NVS Example");

    while (1) {
        uint8_t current_state = read_state();
        uint8_t new_state = (current_state == 1) ? 0 : 1;
        store_state(new_state);

        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }

    return 0;
}
