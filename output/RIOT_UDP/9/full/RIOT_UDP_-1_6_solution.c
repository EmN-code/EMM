#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "esp_wifi.h"
#include "dht.h"
#include "mutex.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "dht_params.h"
#include "esp_system.h"
#include "timex.h"
#include "stddef.h"
#include "arpa/inet.h"
#include "stdio.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "netinet/in.h"
#include "net/ipv6/addr.h"
#include "stdlib.h"
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "unistd.h"
#include "event/thread.h"

#define SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"
#define BUFFER_SIZE 128
#define DELAY 5

static const char *device_state = "work";

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton error");
        close(sock);
        return -1;
    }

    // Infinite loop to send UDP packet every 5 seconds
    while (1) {
        ssize_t bytes_sent = sendto(sock, device_state, strlen(device_state), 0,
                                   (const struct sockaddr *)&server_addr, sizeof(server_addr));
        if (bytes_sent < 0) {
            perror("sendto failed");
        } else {
            printf("Sent %ld bytes to %s:%d\n", bytes_sent, SERVER_IP, SERVER_PORT);
        }

        // Wait for the defined delay
        xtimer_sleep(DELAY);
    }

    // Close socket (unreachable in this setup since it's a continuous loop)
    close(sock);
    return 0;
}
