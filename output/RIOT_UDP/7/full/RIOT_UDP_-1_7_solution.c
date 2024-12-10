#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mutex.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "Device state: work"

int udp_send(const char *ip, uint16_t port, const char *message);

int main(void) {
    while (1) {
        int rc = udp_send(SERVER_IP, SERVER_PORT, MESSAGE);
        if (rc < 0) {
            printf("Failed to send UDP packet. Error: %d\n", rc);
        } else {
            printf("UDP packet sent successfully\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}

int udp_send(const char *ip, uint16_t port, const char *message) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = port };
    inet_pton(AF_INET, ip, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        return -1;
    }

    ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);
    sock_udp_close(&sock);

    return res;
}