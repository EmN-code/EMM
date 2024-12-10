#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "string.h"
#include "thread.h"
#include "net/ipv6/addr.h"

#define BROKER_PORT     1883
#define BROKER_ADDRESS  "47.102.103.1"
#define BUFFER_SIZE     128
#define STATUS_TOPIC    "device/status"
#define PUB_INTERVAL    (5U) // in seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];

static emcute_topic_t status_topic;

void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };

    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_ADDRESS);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error on connecting: %d\n", res);
        return NULL;
    }

    status_topic.name = STATUS_TOPIC;
    res = emcute_reg(&status_topic);
    if (res != EMCUTE_OK) {
        printf("Error on topic registration: %d\n", res);
        return NULL;
    }

    while (1) {
        const char *status_msg = "work"; // example status
        res = emcute_pub(&status_topic, status_msg, strlen(status_msg), 0);
        if (res != EMCUTE_OK) {
            printf("Error publishing: %d\n", res);
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    emcute_discon();
    return NULL;
}

int main(void) {
    puts("MQTT-SN example using emcute for RIOT OS");

    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_thread, NULL, "mqtt");

    return 0;
}