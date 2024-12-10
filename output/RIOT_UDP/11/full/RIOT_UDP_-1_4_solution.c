#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"
#include "udp.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "esp_system.h"
#include "net/af.h"
#include "netinet/in.h"

#define UDP_SERVER_PORT 20001
#define UDP_SERVER_ADDR "47.102.103.1"
#define SEND_INTERVAL (5U)

typedef enum {
    DEVICE_STATE_WORK,
    DEVICE_STATE_NOT_WORK
} device_state_t;

static void *udp_state_sender_thread(void *arg) {
    (void) arg;
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = UDP_SERVER_PORT };

    int rc;
    while (1) {
        rc = sock_udp_create(&sock, NULL, NULL, 0);
        if (rc < 0) {
            printf("Error creating UDP sock: %d\n", rc);
            continue;
        }

        rc = sock_udp_str2ep(&remote, UDP_SERVER_ADDR);
        if (rc != 0) {
            printf("Error parsing destination address\n");
            sock_udp_close(&sock);
            continue;
        }

        const char *state_str = (device_state == DEVICE_STATE_WORK) ? "work" : "not work";

        rc = sock_udp_send(&sock, state_str, strlen(state_str), &remote);
        if (rc < 0) {
            printf("Error sending data: %d\n", rc);
        } else {
            printf("State sent: %s\n", state_str);
        }

        sock_udp_close(&sock);

        for (unsigned i = 0; i < SEND_INTERVAL; ++i) {
            xtimer_sleep(1);
        }
    }

    return NULL;
}

int main(void) {
    static char udp_sender_stack[THREAD_STACKSIZE_MAIN];
    thread_create(udp_sender_stack, sizeof(udp_sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, udp_state_sender_thread, NULL, "udp_sender");

    return 0;
}