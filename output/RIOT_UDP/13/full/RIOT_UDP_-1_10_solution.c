#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/ipv6/addr.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "dht_params.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "udp.h"
#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define SLEEP_TIME (5U * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t remote;

int main(void) {
    sock_udp_t sock;
    uint8_t buf[64];
    ssize_t res;
    unsigned attempts;

    inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6);
    remote.port = SERVER_PORT;
    remote.family = AF_INET6;
    
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Could not create UDP socket\n");
        return 1;
    }

    while (1) {
        attempts = 0;
        while (1) {
            res = sock_udp_send(&sock, MESSAGE, sizeof(MESSAGE), &remote);
            if (res < 0) {
                printf("Error sending message\n");
                attempts++;
                if (attempts >= 5) {
                    printf("Max attempts reached, retrying in 1 second\n");
                    sleep(1);
                    attempts = 0;
                }
            } else {
                printf("Message sent\n");
                break;
            }
        }

        usleep(SLEEP_TIME);
    }

    return 0;
}