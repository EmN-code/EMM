#include "net/asymcute_conf.h"
#include "net/emcute.h"
#include "msg.h"
#include "xtimer.h"

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define CLIENT_ID "riot-esp32-client"
#define STATE_TOPIC "state"
#define MSG_TYPE_WORK "work"
#define MSG_TYPE_NOT_WORK "not_work"

#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static char *client_id = CLIENT_ID;

static msg_t queue[8];

void *publisher_thread(void *arg) {
    (void)arg;

    char *state = MSG_TYPE_WORK; // Initial state

    while (1) {
        emcute_msg_t msg;
        msg.data = (void *)state;
        msg.len = state == MSG_TYPE_WORK ? strlen(MSG_TYPE_WORK) : strlen(MSG_TYPE_NOT_WORK);

        if (emcute_pub(&topic, &msg, EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data
");
        } else {
            printf("Published: %s
", state);
        }

        // Toggle state for the next publish
        state = (state == MSG_TYPE_WORK) ? MSG_TYPE_NOT_WORK : MSG_TYPE_WORK;

        // Wait for the next publish interval
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    printf("MQTT-SN client application
");

    // Connect to MQTT-SN broker
    emcute_con_t connection;
    connection.port = MQTT_SN_BROKER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&connection.addr, MQTT_SN_BROKER_IP);

    if (emcute_connect(&connection, true, client_id, strlen(client_id)) != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker
");
        return 1;
    }

    printf("Connected to MQTT-SN broker at %s:%d
", MQTT_SN_BROKER_IP, MQTT_SN_BROKER_PORT);

    // Initialize topic
    topic.name = STATE_TOPIC;
    topic.id = 0;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic
");
        return 1;
    }

    printf("Registered topic: %s
", STATE_TOPIC);

    // Start the publisher thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  publisher_thread, NULL, "publisher_thread");

    return 0;
}