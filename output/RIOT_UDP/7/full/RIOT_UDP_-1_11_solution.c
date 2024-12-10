#include "ztimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "string.h"
#include "unistd.h"
#include "msg.h"
#include "mutex.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define BUF_SIZE 128

static sock_udp_t sock;
static sock_udp_ep_t remote;

void *udp_sender_thread(void *arg) {
    (void)arg;
    char buffer[BUF_SIZE];
    strcpy(buffer, "work");  // State to send

    while (1) {
        ssize_t res = sock_udp_send(&sock, buffer, sizeof(buffer), &remote);
        if (res < 0) {
            printf("Error sending data\n");
            continue;
        }
        else {
            printf("Data sent to %s:%d\n", SERVER_IP, SERVER_PORT);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    memset(&remote, 0, sizeof(remote));

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, udp_sender_thread, NULL, "udp_sender");

    return 0;
}