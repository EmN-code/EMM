#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

void* publish_state(void *arg) {
    (void)arg;
    const char *messages[] = { "work", "not work" };
    int current_state = 0;
    
    while (1) {
        // Toggle state: "work" <-> "not work"
        current_state = 1 - current_state;

        while (emcute_pub(&topic, messages[current_state], strlen(messages[current_state]), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Publish failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }

        printf("Published: %s\n", messages[current_state]);

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    while (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Connection to broker failed, retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    printf("Connected to broker\n");

    topic.name = MQTT_TOPIC;
    topic.id = 0;   // Assume the broker assigns an ID for topic registration

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publish_state, NULL, "pub");

    while (1) {
        // main thread can perform other tasks
        ztimer_sleep(ZTIMER_MSEC, 10000);
    }

    return 0;
}