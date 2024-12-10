#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/mqtt.h>
#include <net/emcute.h>
#include <msg.h>
#include <net/sock/udp.h>
#include <thread.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)
#define MQTT_TOPIC "state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void *mqtt_thread(void *arg) {
    (void) arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    ipv6_addr_t addr;

    inet_pton(AF_INET, SERVER_ADDR, &addr);
    memcpy(&gw.addr.ipv6, &addr, sizeof(addr));

    mqtt_client_t client;
    mqtt_pkt_t pkt;
    char *pub_msg_work = "work";
    char *pub_msg_not_work = "not work";

    mqtt_client_init(&client, &pkt, queue, ARRAY_SIZE(queue), "esp32");

    while (1) {
        char *state = (xtimer_now_usec() % 10 < 5) ? pub_msg_work : pub_msg_not_work;
        mqtt_publish(&client, MQTT_TOPIC, state, strlen(state), 0);
        printf("Published state: %s\n", state);
        xtimer_sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}