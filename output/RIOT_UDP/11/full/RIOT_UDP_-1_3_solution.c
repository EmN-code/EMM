#include <udp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/af.h>
#include <event/thread.h>

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT  20002
#define BUFFER_SIZE 64

void *udp_sender(void *arg) {
    (void)arg;
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "Device state: %s", "work");

    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = LOCAL_PORT;

    sock_udp_ep_t remote;
    if (sock_udp_str2ep(&remote, SERVER_IP) < 0) {
        printf("Error setting IP address\n");
        return NULL;
    }
    remote.port = SERVER_PORT;

    sock_udp_t sock;
    ssize_t res;

    while (1) {
        if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
            printf("Failed to create UDP socket\n");
            continue;
        }

        res = sock_udp_send(&sock, buffer, strlen(buffer), &remote);
        if (res < 0) {
            printf("Failed to send data: %d\n", res);
            sock_udp_close(&sock);
            xtimer_sleep(1);
            continue;
        }

        printf("Data sent: %s\n", buffer);
        sock_udp_close(&sock);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create_udp_sender();
    return 0;
}

void thread_create_udp_sender(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), 0, THREAD_CREATE_STACKTEST, udp_sender, NULL, "udp_sender");
}