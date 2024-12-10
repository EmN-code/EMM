#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "netinet/in.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "dht_params.h"
#include "esp_system.h"
#include "timex.h"
#include "dht.h"
#include "stddef.h"
#include "arpa/inet.h"
#include "stdio.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "mutex.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT (20001)
#define MESSAGE "work"

void udp_send_message(sock_udp_ep_t *remote)
{
    sock_udp_t sock;
    ssize_t res;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    while (1) {
        res = sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), remote);
        if (res < 0) {
            printf("Error sending message: %d\n", res);
        } else {
            printf("Message sent: %s\n", MESSAGE);
        }

        ztimer_sleep(ZTIMER_SEC, 5);
    }

    sock_udp_close(&sock);
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: Unable to parse IP address");
        return -1;
    }

    udp_send_message(&remote);

    return 0;
}