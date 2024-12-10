#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"
#define PUB_INTERVAL (5U)

static emcute_topic_t topic;
static char message[64];

static void *mqtt_thread(void *arg) {
    (void)arg;
    emcute_conn_t conn;
    conn.remote_port = BROKER_PORT;

    // Connect to the broker
    ipv6_addr_from_str((ipv6_addr_t *)&conn.remote_addr, BROKER_IP);
    if (emcute_con(&conn) != EMCUTE_OK) {
        puts("Could not connect to broker.");
        return NULL;
    }
    puts("Connected to broker.");

    // Setup the topic
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the broker

    while (1) {
        snprintf(message, sizeof(message), "work or not work");

        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message.");
        }
        else {
            puts("Message published.");
        }

        // Sleep for PUB_INTERVAL seconds
        xtimer_sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), thread_create(), 
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
                  mqtt_thread, NULL, "mqtt");
    return 0;
}