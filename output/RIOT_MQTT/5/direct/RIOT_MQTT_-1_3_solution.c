#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mqtt.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/state"

#define MQTT_BUFFER_SIZE 128
#define MSG_QUEUE_SIZE 8


static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[MSG_QUEUE_SIZE];

static emcute_topic_t subscription;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *msg = (char *)data;
    printf("Received publication on topic "%s": %s\n", topic->name, msg);
}

static int publish(const char *state) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("[MQTT] Error registering topic.\n");
        return 1;
    }

    if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
        puts("[MQTT] Error publishing data.\n");
        return 1;
    }

    printf("[MQTT] Published `%s` to topic '%s'\n", state, TOPIC);
    return 0;
}

static void *pub_thread(void *arg) {
    (void)arg;
    char *states[] = {"work", "not work"};
    size_t state_idx = 0;

    while (1) {
        publish(states[state_idx]);
        state_idx = (state_idx + 1) % 2; // Toggle between 0 and 1
        xtimer_sleep(5);
    }

    return NULL;
}

static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6 };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP) == NULL) {
        puts("[MQTT] Error: invalid address\n");
        return 1;
    }
    gw.port = BROKER_PORT;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[MQTT] Error connecting to broker\n");
        return 1;
    }

    puts("[MQTT] Successfully connected to broker\n");
    return 0;
}

int main(void) {
    msg_init_queue(queue, MSG_QUEUE_SIZE);

    if (connect_broker() != 0) {
        return 1;
    }

    kernel_pid_t pub_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                                THREAD_CREATE_STACKTEST, pub_thread, NULL, "pub_thread");

    if (pub_thread_pid <= KERNEL_PID_UNDEF) {
        puts("Error creating thread\n");
        return 1;
    }

    /* Main thread does nothing. The pub_thread handles publishing. */
    return 0;
}
