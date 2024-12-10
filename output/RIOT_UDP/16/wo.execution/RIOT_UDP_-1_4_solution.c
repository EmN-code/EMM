#include "net/emcute.h"
#include "netinet/in.h"
#include "net/ipv6/addr.h"
#include "dht_params.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "mutex.h"
#include "net/gcoap.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 20002

void *udp_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t local = { .port = LOCAL_PORT };
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    char message[] = "Device state: work";

    int res = sock_udp_create(&sock, &local, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket\n");
        return NULL;
    }

    while (1) {
        ssize_t bytes_sent = sock_udp_send(&sock, message, sizeof(message), &remote);
        if (bytes_sent < 0) {
            printf("Error sending message\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, udp_thread, NULL, "udp_thread");
    return 0;
}