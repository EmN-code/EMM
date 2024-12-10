
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <time_units.h>
#include <esp_system.h>
#include <xtimer.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <stdio.h>
#include <dht_params.h>
#include <stdatomic.h>
#include <ztimer.h>
#include <dht.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <net/sock.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <architecture.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;

static int connect_to_broker(void) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    sock_tcp_create(&sock, NULL, (struct sockaddr *)&server_addr, SOCK_FLAGS_REUSE_EP);
    return sock_tcp_connect(&sock, (struct sockaddr *)&server_addr, SOCK_FLAGS_REUSE_EP);
}

static int send_message(const char *payload, size_t len) {
    return sock_tcp_write(&sock, payload, len);
}

static int receive_message(char *buffer, size_t len) {
    return sock_tcp_read(&sock, buffer, len);
}

int main(void) {
    if (connect_to_broker() < 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        if (send_message(PAYLOAD, PAYLOAD_LEN) < 0) {
            printf("Failed to send message\n");
        } else {
            printf("Message sent: %s\n", PAYLOAD);
        }

        xtimer_usleep(INTERVAL);
    }

    sock_tcp_disconnect(&sock);
    return 0;
}
