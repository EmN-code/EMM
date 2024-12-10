#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
#define BROKER_PORT         (1883)
#define BROKER_ADDRESS      "47.102.103.1"
#define MQTT_TOPIC          "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[MAIN_QUEUE_SIZE];
static emcute_topic_t topic;

static bool continue_publishing = true;

void *publisher_thread(void *arg) {
    (void)arg;
    char message[12];
    while (continue_publishing) {
        snprintf(message, sizeof(message), "work");
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish data\n");
        } else {
            printf("Published data successfully\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, BROKER_ADDRESS);

    char *client_id = "mqtt-sn-device";

    while (true) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Connected to broker\n");
            break;
        } else {
            printf("Connection failed, retrying...\n");
            xtimer_sleep(1);
        }
    }

    topic.name = MQTT_TOPIC;
    topic.id = 0;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread,
                  NULL, "publisher");

    return 0;
}
