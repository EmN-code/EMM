#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/mqtt.h"
#include "net/ipv4/addr.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define MQTT_INTERVAL (5 * 1000000) // 5 seconds in microseconds

static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];
static struct sockaddr_in broker_addr;

static void on_puback(mqtt_client_t *client, void *data, uint16_t packet_id) {
    (void)client;
    (void)data;
    (void)packet_id;
    printf("Published message acknowledged\n");
}

static void on_connect(mqtt_client_t *client, void *data, mqtt_connection_state_t state) {
    (void)client;
    (void)data;
    if (state == MQTT_CONNECT_ACCEPTED) {
        printf("Connected to MQTT broker\n");
    } else {
        printf("Connection to MQTT broker failed\n");
    }
}

static void on_disconnect(mqtt_client_t *client, void *data, mqtt_connection_state_t state) {
    (void)client;
    (void)data;
    printf("Disconnected from MQTT broker with state %d\n", state);
}

static void on_subscribe(mqtt_client_t *client, void *data, uint16_t packet_id, const uint8_t *granted_qos, size_t len) {
    (void)client;
    (void)data;
    (void)packet_id;
    (void)granted_qos;
    (void)len;
    printf("Subscription acknowledged\n");
}

static void on_unsubscribe(mqtt_client_t *client, void *data, uint16_t packet_id) {
    (void)client;
    (void)data;
    (void)packet_id;
    printf("Unsubscription acknowledged\n");
}

static void on_publish(mqtt_client_t *client, void *data, const mqtt_topic_t *topic, const uint8_t *data, size_t len, int qos, bool retain) {
    (void)client;
    (void)data;
    (void)topic;
    (void)data;
    (void)len;
    (void)qos;
    (void)retain;
    printf("Received published message\n");
}

static void setup_mqtt_client() {
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    mqtt_client_init(&client, NULL, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), on_connect, on_disconnect, on_publish, on_puback, on_subscribe, on_unsubscribe, NULL, NULL);

    mqtt_conn_params_t conn_params = {
        .host = (struct sockaddr *)&broker_addr,
        .host_len = sizeof(broker_addr),
        .keep_alive = 60,
        .clean_session = true,
        .will_retain = false,
        .will_qos = MQTT_QOS_0,
        .will_topic = NULL,
        .will_message = NULL,
        .will_message_len = 0,
        .username = NULL,
        .password = NULL,
    };

    mqtt_connect(&client, &conn_params, NULL);
}

static void publish_state(bool working) {
    const char *message = working ? MQTT_MESSAGE_WORK : MQTT_MESSAGE_NOT_WORK;
    mqtt_topic_t topic = MQTT_TOPIC;

    mqtt_message_t msg = {
        .qos = MQTT_QOS_0,
        .retain = false,
        .data = (uint8_t *)message,
        .len = strlen(message),
    };

    mqtt_publish(&client, &topic, &msg, NULL);
}

int main(void) {
    setup_mqtt_client();

    while (1) {
        mqtt_yield(&client, MQTT_INTERVAL);

        // Simulate state (for demonstration purposes)
        static bool working = true;
        publish_state(working);
        working = !working; // Toggle state

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}