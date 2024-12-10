#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MSG_CONTENT "work or not work"
#define PUBLISH_INTERVAL (5U * MS_PER_SEC)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_LARGE];

static const sock_udp_ep_t broker = {
    .family = AF_INET, // Correctly specify the address family
    .port = MQTT_BROKER_PORT
};

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(MQTT_BROKER_PORT, "mqtt-client");
    return NULL;
}

int main(void) {
    msg_init_queue(queue, sizeof(queue) / sizeof(queue[0]));

    if (inet_pton(AF_INET, MQTT_BROKER_ADDRESS, &broker.addr.ipv4) != 1) {
        puts("Error: unable to parse broker address");
        return 1;
    }

    if (emcute_con((sock_udp_ep_t *)&broker, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    // Initialize the subscription topic
    topic.name = "state";
    topic.id = 0;  // Topic ID will be filled by emcute_reg
    emcute_reg(&topic);

    // Publish the message every 5 seconds
    while (1) {
        if (emcute_pub(&topic, MSG_CONTENT, strlen(MSG_CONTENT), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    // The emcute_thread will handle incoming subscriptions or messages
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");
    if (pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to start emcute thread");
        return 1;
    }

    return 0;
}