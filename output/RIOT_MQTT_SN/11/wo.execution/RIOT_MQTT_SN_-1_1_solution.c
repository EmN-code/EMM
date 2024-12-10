#include <xtimer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "net/emcute.h"

#define BROKER_ADDRESS  "47.102.103.1"
#define BROKER_PORT     1883
#define TOPIC_NAME      "device/status"
#define MESSAGE_INTERVAL (5U * US_PER_SEC)

static emcute_con_t connection;
static emcute_topic_t topic;

static void publish_state(void) {
    char *messages[] = { "work", "not work" };
    char *message = messages[rand() % 2];  // Randomly choose message
    emcute_msg_t emcute_message;

    emcute_message.data = message;
    emcute_message.len = strlen(message);
    emcute_message.flags = EMCUTE_MSG_FLAG_RETAIN;

    if (emcute_pub(&topic, &emcute_message, EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
    } else {
        printf("Published: %s\n", message);
    }
}

static void *thread_pub(void *arg) {
    (void)arg;
    while (1) {
        publish_state();
        xtimer_sleep(MESSAGE_INTERVAL);
    }
    return NULL;
}

static int setup_connection(void) {
    memset(&connection, 0, sizeof(connection));
    connection.addr.family = AF_INET;
    if (inet_pton(AF_INET, BROKER_ADDRESS, &connection.addr.ipv4) != 1) {
        printf("Error: unable to parse broker address\n");
        return 1;
    }
    connection.port = BROKER_PORT;
    return emcute_con(&connection) != EMCUTE_OK ? 1 : 0;
}

int main(void) {
    printf("Configuring MQTT-SN...");

    if (setup_connection() != 0) {
        printf("Error: unable to setup connection\n");
        return 1;
    }

    topic.name = TOPIC_NAME;
    topic.id = 0;

    // Publish first message
    publish_state();

    // Launch the publishing thread
    thread_t pub_thread;
    thread_create(pub_thread_stack, sizeof(pub_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_pub, NULL, "pub");

    // Continue doing other tasks
    while (1) {
        // Ideally listen and process other possible interactions
        xtimer_sleep(60);
    }

    return 0;
}