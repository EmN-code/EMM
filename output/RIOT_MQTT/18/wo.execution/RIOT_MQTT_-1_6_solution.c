#include <ztimer.h>
#include <arpa/inet.h>
#include <evtimer_msg.h>
#include <timex.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <kernel_defines.h>
#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <dht_params.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <architecture.h>
#include <sys/socket.h>
#include <msg.h>
#include <mbox.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <net/emcute.h>
#include <event/thread.h>
#include <MQTTClient.h>
#include <net/mqttsn.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "device/status"
#define INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];

void *publisher_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    while (1) {
        char status_msg[15];
        snprintf(status_msg, sizeof(status_msg), "work");
        /* here would be the code to send the message using MQTT library */
        printf("Publishing message: %s\n", status_msg);
        /* sleep for the defined interval */
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("MQTT publisher example");
    /* Start publishing thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publisher_thread, NULL, "publisher");
    return 0;
}