#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "unistd.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "kernel_defines.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUF_SIZE 64
#define STATE_MESSAGE "work or not work"

void *send_udp_status(void *arg) {
    (void)arg;
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE] = STATE_MESSAGE;

    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        printf("Socket creation error\n");
        return NULL;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        close(sock);
        return NULL;
    }

    while (1) {
        if (sendto(sock, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            printf("Send failed\n");
        } else {
            printf("State sent: %s\n", buffer);
        }
        // wait for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    close(sock);
    return NULL;
}

int main(void) {
    thread_create(buffer, sizeof(buffer), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, send_udp_status, NULL, "send_udp_status");
    return 0;
}