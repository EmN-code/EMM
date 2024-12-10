#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "esp_system.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "thread.h"

#define SERVER_PORT 20001
#define SERVER_IP "47.102.103.1"
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];

void *udp_sender(void *arg) {
    (void) arg;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return NULL;
    }

    while (1) {
        const char *data = "work";
        if (sock_udp_send(&sock, data, strlen(data), &remote) < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("Sent: %s\n", data);
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  udp_sender, NULL, "udp_sender");
    return 0;
}