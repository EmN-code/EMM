#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "MQTTClient.h"  // Ensure this is the last header file included

#define EMCUTE_ID           ("deviceID")
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define BROKER_PORT         1883
#define BROKER_ADDRESS      "47.102.103.1"
#define PUB_TOPIC           "state"
#define PUB_INTERVAL        (5U * US_PER_SEC)  // 5 seconds interval

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static MQTTClient client;
static Network network;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void on_message(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received message: %.*s\n", (int)len, (char *)data);
}

int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_ADDRESS);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error: unable to connect to broker\n");
        return res;
    }
    return EMCUTE_OK;
}

void *publish_state(void *arg) {
    while (1) {
        if (!MQTTIsConnected(&client)) {
            if (connect_to_broker() != EMCUTE_OK) {
                xtimer_sleep(1);
                continue;
            }
        }
        const char* message = "work";  // Change to "not work" as required
        MQTTPublish(&client, PUB_TOPIC, (MQTTMessage*)&message);
        printf("Published: %s\n", message);
        xtimer_usleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publish_state, NULL, "mqtt_publisher");
    return 0;
}