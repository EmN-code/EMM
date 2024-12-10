#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "thread.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL 5

static emcute_topic_t topic;

void *mqtt_publish_thread(void *arg) {
    // publish "work or not work" every PUBLISH_INTERVAL seconds
    const char *message = "work or not work";
    while (1) {
        int res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish the message (%d)\n", res);
        }
        sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse MQTT broker address\n");
        return -1;
    }

    while (1) {
        int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to connect to the broker (%d). Retrying...\n", res);
            sleep(1);
            continue;
        }
        break;
    }

    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    topic.name[sizeof(topic.name) - 1] = '\0'; // Ensure null-termination

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic (%d)\n", res);
        return -1;
    }

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_publish_thread, NULL, "mqtt_publish_thread");

    // The main thread can be put to sleep indefinitely as all operations are handled by the publish thread
    thread_sleep();

    return 0;
}