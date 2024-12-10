#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/mqttsn.h"

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/status"
#define MESSAGE "work"
#define NOT_MESSAGE "not work"

static void on_pub_lost(void *arg, const emcute_topic_t *topic, uint16_t short_id) {
    (void)topic;
    (void)short_id;
    (void)arg;
    printf("Publish lost\n");
}

static emcute_sub_t subscriptions[1];
static char status[10] = "work";

int main(void) {
    emcute_con_t conn;
    conn.addr_family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_ADDRESS, &conn.addr.ipv6);
    conn.port = MQTT_BROKER_PORT;

    conn.status_callback = NULL;
    conn.status_callback_status = NULL;

    conn.sec = 0;

    mqttsn_pub_data_t data;
    emcute_topic_t topic;

    /* connect to broker */
    if (mqttsn_connect(&conn) != 0) {
        printf("Unable to connect\n");
        return 1;
    }

    topic.name = MQTT_TOPIC;
    topic.id = 0;

    subscriptions[0].cb = on_pub_lost;
    subscriptions[0].topic.name = MQTT_TOPIC;

    while (1) {
        /* publish data */
        strncpy(data.payload, status, strlen(MESSAGE));
        data.payload_size = strlen(status);
        data.flags = MQTT_SN_TOPICID_LONG;

        if (mqttsn_publish(&conn, &topic, &data) != EMCUTE_OK) {
            printf("Error publishing data\n");
        } else {
            printf("Published: %s\n", data.payload);
        }

        /* Toggle status */
        if (strcmp(status, "work") == 0) {
            strcpy(status, "not work");
        } else {
            strcpy(status, "work");
        }

        xtimer_sleep(5);
    }

    /* disconnect from broker */
    mqttsn_disconnect(&conn);
    return 0;
}