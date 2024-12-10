#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC       "state"
#define PUBLISH_INTERVAL (5 * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;
static char state_msg[32];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Received MQTT-SN message\n");
}

static void *mqtt_thread(void *arg) {
    emcute_con_t conn;
    int conn_status;

    /* Initialize the MQTT-SN client */
    memset(&conn, 0, sizeof(conn));
    emcute_init();

    /* Prepare connection parameters */
    conn.remote.sa_family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_IP, &conn.remote.sin_addr);
    conn.remote.sin_port = htons(MQTT_BROKER_PORT);

    /* Connect to the MQTT-SN broker */
    while (1) {
        conn_status = emcute_con(&conn, true, "RIOT-ESP32");
        if (conn_status == EMCUTE_OK) {
            printf("Connected to MQTT-SN broker\n");
            break;
        } else {
            printf("Failed to connect, retrying in 1 second\n");
            xtimer_sleep(1);
        }
    }

    /* Set the topic name and ID */
    topic.name = MQTT_TOPIC;
    topic.id = 0;
    emcute_reg(&topic);

    /* Infinite loop to publish messages every 5 seconds */
    while (1) {
        sprintf(state_msg, "work"); // Example state message
        emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        printf("Published message: %s\n", state_msg);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    /* Disconnect from MQTT-SN broker */
    emcute_discon();

    return NULL;
}

int main(void) {
    /* Create a thread to manage MQTT-SN connection and publication */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    /* Let the main thread enter an idle loop */
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}