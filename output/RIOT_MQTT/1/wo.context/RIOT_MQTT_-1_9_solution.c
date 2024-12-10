#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/tcp.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define MAIN_QUEUE_SIZE     (8)
#define SERVER_ADDRESS      "47.102.103.1"
#define SERVER_PORT         (1883)
#define PUBLISH_INTERVAL    (5U * US_PER_SEC) // Publish every 5 seconds

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void on_message_receive(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;  // Unused in this example
    (void)data;   // Unused in this example
    (void)len;    // Unused in this example
}

int main(void) {
    printf("RIOT MQTT publish example for ESP32\n");
    
    emcute_sub_t subscription;
    subscription.topic.name = "state";
    subscription.cb = on_message_receive;

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDRESS, &remote.addr.ipv4);

    emcute_con(&remote, true, NULL, NULL, 0, 0);

    while (1) {
        char *msg = "work";  // Or "not work" based on some condition updating msg
        emcute_topic_t publish_topic;
        publish_topic.name = "state";

        if (emcute_pub(&publish_topic, msg, strlen(msg), EMCUTE_QOS_0)== EMCUTE_OK) {
            printf("Published state: %s\n", msg);
        }
        else {
            printf("Failed to publish\n");
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}