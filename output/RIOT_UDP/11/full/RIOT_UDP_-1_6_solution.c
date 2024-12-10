#include <ztimer.h>  
#include <stdbool.h>  
#include <stdatomic.h>  
#include <stddef.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <time_units.h>  
#include <timex.h>  
#include <evtimer_mbox.h>  
#include <evtimer_msg.h>  
#include <msg.h>  
#include <esp_system.h>  
#include <esp_wifi.h>  
#include <mbox.h>  
#include <MQTTClient.h>  
#include <dht_params.h>  
#include <kernel_defines.h>  
#include <mutex.h>
#include <thread.h>  
#include <event/thread.h>  
#include <net/sock/udp.h>  
#include <net/sock/util.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <net/emcute.h>  
#include <net/gcoap.h>  
#include <net/af.h>  
#include <xtimer.h>  
#include <paho_mqtt.h>  

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

static sock_udp_ep_t server;
static sock_udp_t sock;

void udp_connect(void) {
    while (1) {
        if (sock_udp_create(&sock, NULL, NULL, 0) == 0) {
            printf("UDP socket created\n");
            break;
        }
        printf("Failed to create UDP socket, retrying...\n");
        ztimer_sleep(ZTIMER_USEC, US_PER_SEC);
    }
}

void udp_send(const char *data) {
    while (1) {
        ssize_t res = sock_udp_send(&sock, data, strlen(data), &server);
        if (res >= 0) {
            printf("Data sent: %s\n", data);
            break;
        }
        printf("Failed to send data, retrying...\n");
        ztimer_sleep(ZTIMER_USEC, US_PER_SEC);
    }
}

int main(void) {
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    server.family = AF_INET;
    server.port = SERVER_PORT;

    udp_connect();
    
    while (1) {
        const char *state = "work"; // or "not work"
        udp_send(state);

        ztimer_sleep(ZTIMER_USEC, SEND_INTERVAL);
    }

    return 0;
}