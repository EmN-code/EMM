#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <thread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <net/sock/udp.h>

// Address of the MQTT Broker
#define BROKER_IP      "47.102.103.1"
#define BROKER_PORT    1883

// Message state
#define WORK_STATE     "work"
#define NOT_WORK_STATE "not work"

// Interval for sending messages (in microseconds)
#define SEND_INTERVAL  5000000L

void *publish_thread(void *arg) {
    (void)arg;
    struct sockaddr_in6 server_addr;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = htons(BROKER_PORT) };
    inet_pton(AF_INET6, BROKER_IP, &server_addr.sin6_addr.s6_addr);
    remote.addr.ipv6 = server_addr.sin6_addr;

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    char *state = WORK_STATE;
    while (1) {
        if (sock_udp_send(&sock, state, strlen(state), &remote) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Message '%s' sent\n", state);
        }
        state = (state == WORK_STATE) ? NOT_WORK_STATE : WORK_STATE;
        usleep(SEND_INTERVAL);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publish_thread, NULL, "publish_thread");

    return 0;
}