#include "net/gcoap.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "thread.h"
#include "ztimer.h"
#include "xtimer.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "time_units.h"
#include "timex.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "stdio.h"

#define REMOTE_ADDR      "47.102.103.1"
#define REMOTE_PORT      20001
#define MESSAGE_INTERVAL 5

static int create_udp_socket(void) {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        return -1;
    }
    return sock;
}

static void udp_send(int socket, const char *message, struct sockaddr_in *addr) {
    sendto(socket, message, strlen(message), 0, (struct sockaddr *)addr, sizeof(*addr));
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    char *message = "device state: work";

    sock = create_udp_socket();
    if (sock < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(REMOTE_PORT);
    inet_pton(AF_INET, REMOTE_ADDR, &server_addr.sin_addr);

    while (1) {
        udp_send(sock, message, &server_addr);
        sleep(MESSAGE_INTERVAL);
    }

    close(sock);
    return 0;
}