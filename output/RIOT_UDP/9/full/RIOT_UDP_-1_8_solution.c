#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "netinet/in.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "pthread.h"
#include "timex.h"
#include "mqttclient.h"
#include "m.mqtt.h"
#include "mutex.h"
#include "thread.h"
#include "unistd.h"

#define RECONNECT_INTERVAL    (1U)
#define SEND_INTERVAL         (5U)
#define SERVER_PORT           20001
#define SERVER_IP             "47.102.103.1"
#define UDP_BUFFER_SIZE       64

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    uint8_t buffer[UDP_BUFFER_SIZE];
    char *device_state = "work";
    int res;

    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        puts("Failed to convert IP address");
        return 1;
    }

    while (1) {
        res = sock_udp_create(&sock, NULL, &remote, 0);
        if (res != 0) {
            puts("Failed to create UDP socket");
            ztimer_sleep(RECONNECT_INTERVAL);
            continue;
        }

        size_t len = snprintf((char *)buffer, UDP_BUFFER_SIZE, "Device State: %s", device_state);

        if (sock_udp_send(&sock, buffer, len, &remote) < 0) {
            puts("Failed to send UDP message");
            sock_udp_close(&sock);
            ztimer_sleep(RECONNECT_INTERVAL);
            continue;
        }

        sock_udp_close(&sock);
        ztimer_sleep(SEND_INTERVAL);
    }

    return 0;
}