#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "time_units.h"

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U) // sending interval in seconds

int main(void) {
    int sock;
    struct sockaddr_in6 server_addr;
    
    // Create UDP socket
    sock = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return 1;
    }
    
    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    inet_pton(AF_INET6, SERVER_IP, &server_addr.sin6_addr);
    server_addr.sin6_port = htons(SERVER_PORT);
    
    const char *states[2] = {"work", "not work"};
    unsigned state_index = 0;

    while (1) {
        // Send data to the server
        ssize_t bytes_sent = sendto(sock, states[state_index], strlen(states[state_index]), 0,
                                    (struct sockaddr *)&server_addr, sizeof(server_addr));
        
        if (bytes_sent < 0) {
            printf("Error sending data\n");
        } else {
            printf("Sent: %s\n", states[state_index]);
        }

        // Toggle state
        state_index = 1 - state_index;

        // Wait for the next interval
        ztimer_sleep(ZTIMER_MSEC, SEND_INTERVAL * 1000);
    }

    // Close the socket
    close(sock);
    return 0;
}