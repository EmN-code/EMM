#include <sys/socket.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ztimer.h>
#include <net/emcute.h>
#include <msg.h>
#include <net/sock/udp.h>
#include <mqtt_client.h>
#include <event/callback.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <dht_params.h>
#include <esp_system.h>
#include <timex.h>
#include <dht.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <architecture.h>
#include <mutex.h>
#include <unistd.h>
#include <netinet/in.h>
#include <esp_wifi.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <xtimer.h>

static void event_periodic_callback(void *arg) {
    printf("Callback Triggered\n");
}

void init_event_periodic(void) {
    printf("Starting event periodic timer\n");
    ztimer_t timer;
    ztimer_set_handler(&timer, event_periodic_callback, NULL);
    while (ztimer_periodic_start(&timer, ZTIMER_SEC) != 0) {
        printf("Timer start failed, retrying in 1 second\n");
        xtimer_sleep(1);
    }
    printf("Timer successfully started\n");
}

int main(void) {
    puts("RIOT ESP32 Event Periodic Example\n");
    init_event_periodic();
    while (1) {
        // Main loop can perform other tasks
        xtimer_sleep(1);
    }
    return 0;
}
