#include "thread.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "mutex.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht_params.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/sock/util.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL 5
#define STACKSIZE 1024
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char thread_stack[STACKSIZE];

void *udp_thread(void *arg) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return NULL;
    }

    while (1) {
        const char *message = "work or not work";
        ssize_t res = sock_udp_send(&sock, message, strlen(message), &remote);

        if (res < 0) {
            printf("Error sending UDP packet\n");
        }
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY, 0, udp_thread, NULL, "udp_thread");
    return 0;
}