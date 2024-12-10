#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PUBLISH_INTERVAL (5 * 1000000) // 5 seconds in microseconds

static char sendbuf[128];
static char readbuf[128];
static mqtt_client_t client;

static void on_puback(mqtt_client_t *client, uint16_t packet_id, void *data) {
    (void)client;
    (void)packet_id;
    (void)data;
    printf("PUBACK received\n");
}

static void on_connect(mqtt_client_t *client, uint16_t packet_id, void *data) {
    (void)client;
    (void)packet_id;
    (void)data;
    printf("Connected to MQTT broker\n");

    mqtt_topic_t topics[] = {
        { .topic = MQTT_TOPIC, .qos = MQTT_QOS_0 },
    };

    mqtt_subscribe(&client, topics, ARRAY_SIZE(topics), NULL, NULL);
}

static void on_disconnect(mqtt_client_t *client, void *data) {
    (void)client;
    (void)data;
    printf("Disconnected from MQTT broker\n");
}

static void publish_state(mqtt_client_t *client, const char *state) {
    mqtt_message_t msg = {
        .qos = MQTT_QOS_0,
        .retain = false,
        .dup = false,
        .id = 0,
        .payload = (uint8_t *)state,
        .payload_len = strlen(state),
    };

    mqtt_publish(&client, MQTT_TOPIC, &msg, on_puback, NULL);
}

int main(void) {
    printf("Starting MQTT client...\n");

    // Initialize network
    sock_tcp_t sock = SOCK_TCP_INVALID;
    Network n = {
        .sock = &sock,
        .mqttread = sock_tcp_read,
        .mqttwrite = sock_tcp_write,
    };

    // Initialize MQTT client
    mqtt_client_init(&client, &n, MQTT_PUBLISH_INTERVAL / 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    struct sockaddr_in broker_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(MQTT_BROKER_PORT),
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    if (NetworkConnect(&n, (char *)&broker_addr.sin_addr.s_addr, ntohs(broker_addr.sin_port)) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Set up connection callbacks
    mqtt_set_connect_handler(&client, on_connect, NULL);
    mqtt_set_disconnect_handler(&client, on_disconnect, NULL);

    // Connect to MQTT broker
    mqtt_connect(&client, NULL, NULL, NULL, NULL);

    // Main loop
    while (1) {
        // Check for incoming messages
        mqtt_yield(&client, MQTT_PUBLISH_INTERVAL / 1000);

        // Publish state
        static bool working = true;
        const char *state = working ? "work" : "not work";
        publish_state(&client, state);

        // Toggle state
        working = !working;

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}