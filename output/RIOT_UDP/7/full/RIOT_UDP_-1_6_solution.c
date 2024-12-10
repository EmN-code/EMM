#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "event/thread.h"
#include "kernel_defines.h"
#include "dht_params.h"
#include "dht.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mutex.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define STATE "work"
#define DELAY_SEC 5

int main(void) {
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    
    sock_udp_t sock;
    int rc;
    
    while (1) {
        rc = sock_udp_create(&sock, NULL, NULL, 0);
        if (rc < 0) {
            printf("Failed to create UDP socket\n");
            continue;
        } else {
            break;
        }
    }
    
    while (1) {
        rc = sock_udp_send(&sock, STATE, sizeof(STATE), &server);
        if (rc < 0) {
            printf("Failed to send message\n");
        } else {
            printf("Sent state: %s\n", STATE);
        }
        thread_sleep(DELAY_SEC);
    }
    
    sock_udp_close(&sock);
    return 0;
}