#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "dht_params.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define STATE_MSG_WORK "work"
#define STATE_MSG_NOT_WORK "not work"
#define SLEEP_DURATION (5U * US_PER_SEC)

static sock_udp_ep_t server;

void setup_udp_server(void) {
    server.family = AF_INET;
    server.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
}

void *udp_send_thread(void *arg) {
    (void)arg;
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    const char *state_msg = STATE_MSG_WORK;

    while (1) {
        ssize_t res = sock_udp_send(&sock, state_msg, strlen(state_msg), &server);
        if (res < 0) {
            printf("Error sending message: %ld\n", res);
            // Retry logic could be added here
        }
        printf("Sent: %s\n", state_msg);
        ztimer_sleep(ZTIMER_USEC, SLEEP_DURATION);
    }

    return NULL;
}

int main(void) {
    setup_udp_server();
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, udp_send_thread, NULL, "udp_send");
    return 0;
}