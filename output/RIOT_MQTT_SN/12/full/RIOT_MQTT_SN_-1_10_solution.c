#include "mma8x5x.h"
#include "msg.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME "device/state"
#define STACKSIZE  (THREAD_STACKSIZE_MAIN)
#define MAX_RETRY 5

static char stack[STACKSIZE];
static msg_t queue[8];

static emcute_topic_t topic;

void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: Unable to parse IP address");
        return NULL;
    }

    int retry = 0;
    while (retry < MAX_RETRY) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            puts("Connected to broker");
            break;
        }
        else {
            printf("Connection failed, retrying (%d/%d)\n", retry + 1, MAX_RETRY);
            sleep(1);
            retry++;
        }
    }

    if (retry == MAX_RETRY) {
        puts("Failed to connect to broker after multiple attempts");
        return NULL;
    }

    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name) - 1);
    topic.name[sizeof(topic.name) - 1] = '\0';

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: Unable to register topic");
        return NULL;
    }

    while (true) {
        const char *data = "work or not work";
        if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: Unable to publish data");
        } else {
            puts("Data published");
        }
        sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt-thread");
    return 0;
}
