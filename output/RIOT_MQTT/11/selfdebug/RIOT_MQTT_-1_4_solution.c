#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <msg.h>
#include <xtimer.h>
#include <mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)
#define CLIENT_ID "esp32_mqtt_client"
#define TOPIC "device/state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"

static mqtt_client_t client;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void publish_state(bool work_state) {
    mqtt_message_t message;
    message.payload = work_state ? PAYLOAD_WORK : PAYLOAD_NOT_WORK;
    message.payloadlen = strlen(message.payload);
    message.qos = MQTT_QOS_0;
    message.retained = 0;
    message.dup = 0;

    mqtt_publish(&client, NULL, TOPIC, &message);
    printf("Published state: %s\n", message.payload);
}

int main(void) {
    /* Setup MQTT client */
    mqtt_client_init(&client, CLIENT_ID, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    mqtt_connect(&client);

    /* Loop publishing state */
    while (1) {
        publish_state(true);  // Example: it always publishes 'work'.
        xtimer_sleep(5);
    }

    return 0;
}