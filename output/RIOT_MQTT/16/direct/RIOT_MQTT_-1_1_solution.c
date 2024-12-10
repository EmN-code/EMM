#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "string.h"
#include "net/mqttsn.h"
#include "net/mqttsn_publisher.h"
#include "net/netif.h"
#include "net/sock/udp.h"

#define UDP_PORT 1883
#define BROKER_ADDR "47.102.103.1"
#define TIME_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds
#define TOPIC "device/state"

static sock_udp_ep_t gw = {
    .family = AF_INET6, // using IPv6
    .port = UDP_PORT
};

static mqttsn_publisher_t publisher;

void *publish_thread(void *arg) {
    (void)arg;
    char message[16];
    bool current_state = true;

    while (1) {
        snprintf(message, sizeof(message), "%s", current_state ? "work" : "not work");
        current_state = !current_state;  // Toggle state
        
        mqttsn_publisher_publish(&publisher, TOPIC, message, strlen(message), false);

        printf("Published state: %s\n", message);

        xtimer_sleep(TIME_INTERVAL / 1000000UL);  // Sleep for the specified interval
    }

    return NULL;
}

int main(void) {
    netif_t *netif = netif_get_default();

    if (netif == NULL) {
        puts("Error: No default network interface found\n");
        return -1;
    }

    if (netif_get_addr(netif, &gw.addr, sizeof(gw.addr)) < 0) {
        puts("Error: Unable to get network interface address\n");
        return -1;
    }

    mqttsn_publisher_init(&publisher, &gw, netif);

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  publish_thread, NULL, "publish_thread");

    return 0;
}