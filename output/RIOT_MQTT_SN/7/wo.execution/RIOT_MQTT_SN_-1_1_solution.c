#include "pthread.h"
#include <stdio.h>
#include "net/emcute.h"
#include "xtimer.h"

#define EMCUTE_ID          "riot-node"
#define EMCUTE_PORT        (1883U)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define BROKER_IP          "47.102.103.1"
#define BROKER_PORT        (1883U)
#define PUBLISH_INTERVAL   (5U)
#define TOPIC_NAME         "device/state"

static emcute_sock_t sock;
static uint8_t buf[128];

static emcute_topic_t topic;

void *publish(void *arg) {
    (void)arg;
    while (1) {
        xtimer_sleep(PUBLISH_INTERVAL);
        char *message = "work";
        if (emcute_pub(&sock, &topic, buf, (size_t)snprintf((char *)buf, sizeof(buf), "{\"state\":\"%s\"}", message), EMCUTE_ACK) < 0) {
            printf("[Error] Unable to publish data\n");
        } else {
            printf("[Info] Published data: %s\n", message);
        }
    }
    return NULL;
}

int main(void) {
    pthread_t pub_thread;
    struct emcute_cfg_t emcute_conf;

    // Initialize the emcute library
    sock.mqtt_version = EMCUTE_MQTT_SN_VERSION;
    emcute_conf.proto = EMCUTE_PROTO_UDP;
    emcute_conf.sock = &sock;
    emcute_conf.prio = EMCUTE_PRIO;

    emcute_init(&emcute_conf);

    // Setup the topic
    topic.name = TOPIC_NAME;

    // Connect to the broker
    printf("[Info] Connecting to %s:%d\n", BROKER_IP, BROKER_PORT);
    if (emcute_con(BROKER_IP, BROKER_PORT) < 0) {
        printf("[Error] Could not connect to broker\n");
        return -1;
    }

    printf("[Info] Connected successfully\n");

    // Publish thread
    if (pthread_create(&pub_thread, NULL, publish, NULL) != 0) {
        printf("[Error] Failed to create thread\n");
        return -1;
    }

    pthread_join(pub_thread, NULL);

    return 0;
}