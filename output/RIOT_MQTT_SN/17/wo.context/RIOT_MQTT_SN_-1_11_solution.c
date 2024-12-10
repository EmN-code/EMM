// Full RIOT program using emcute to publish a message every 5 seconds
// Connects to the specified MQTT-SN broker.

// Include necessary libraries in descending alphabetical order
#include <emcute.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>

#define EMCUTE_ID           "my_device"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         1883
#define MQTT_BROKER_IP      "47.102.103.1"
#define INTERVAL            (5U * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };

    // Convert IP address
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    // Initialize the emcute library
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    // Connect to the broker
    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Unable to connect to the broker, retrying...");
        xtimer_sleep(2);
    }

    // Register a topic
    topic.name = "device_state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Unable to obtain topic ID");
        return NULL;
    }

    while (1) {
        const char *message = "work";

        // Publish the message
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Unable to publish data");
        } else {
            puts("Data published successfully");
        }

        // Sleep for the defined interval
        xtimer_usleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Start the emcute thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}