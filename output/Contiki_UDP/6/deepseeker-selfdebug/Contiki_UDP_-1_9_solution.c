
#include <stdio.h>
#include "net/ipv6/simple-udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"
#define INTERVAL_MS 5000000  // 5 seconds

static void udp_send_state(void) {
    static char buffer[16];
    sprintf(buffer, "%s", DEVICE_STATE);

    udp_send(SERVER_IP, SERVER_PORT, buffer, strlen(buffer));
}

int main(void) {
    xtimer_init();

    while (1) {
        udp_send_state();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
