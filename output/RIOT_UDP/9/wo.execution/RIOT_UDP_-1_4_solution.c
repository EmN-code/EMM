#include "thread.h"
#include "mutex.h"
#include "MQTTClient.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "esp_system.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL 5

void *udp_sender_thread(void *arg) {
    (void)arg; // Unused
    sock_udp_ep_t server;
    server.family = AF_INET;
    server.netif = SOCK_ADDR_ANY_NETIF;
    server.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        printf("Error creating UDP socket\n");
        return NULL;
    }

    while (1) {
        const char *msg = "device state: work";
        sock_udp_send(&sock, msg, strlen(msg), &server);
        vTaskDelay(MESSAGE_INTERVAL * 1000 / portTICK_PERIOD_MS);
    }

    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    thread_t udp_thread;
    char udp_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(udp_stack, sizeof(udp_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, udp_sender_thread, NULL, "udp_sender");
    return 0;
}