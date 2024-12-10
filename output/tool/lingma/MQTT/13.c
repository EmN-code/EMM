#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define MQTT_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

static void on_puback(mqtt_client_t *client, void *data, uint16_t packet_id) {
    (void)client;
    (void)data;
    (void)packet_id;
    printf("Published message acknowledged\n");
}

static void on_connect(mqtt_client_t *client, void *data, mqtt_connection_state_t state) {
    (void)client;
    (void)data;
    if (state == MQTT_CS_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else {
        printf("Failed to connect to MQTT broker\n");
    }
}

static void on_disconnect(mqtt_client_t *client, void *data) {
    (void)client;
    (void)data;
    printf("Disconnected from MQTT broker\n");
}

static void publish_state(mqtt_client_t *client, const char *state) {
    mqtt_message_t message = MQTT_MESSAGE(state);
    int res = mqtt_publish(client, MQTT_TOPIC, &message, MQTT_QOS_0, false, 0);
    if (res < 0) {
        printf("Failed to publish message: %d\n", res);
    } else {
        printf("Published message: %s\n", state);
    }
}

int main(void) {
    // Initialize the network
    sock_tcp_t sock;
    sock_tcp_init(&sock);

    // Connect to the MQTT broker
    struct sockaddr_in broker_addr;
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr.s_addr);

    int res = sock_tcp_connect(&sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr), NULL);
    if (res < 0) {
        printf("Failed to connect to MQTT broker: %d\n", res);
        return 1;
    }

    // Initialize the MQTT client
    mqtt_client_init(&client, &sock, MQTT_INTERVAL, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    mqtt_set_connect_handler(&client, on_connect, NULL);
    mqtt_set_disconnect_handler(&client, on_disconnect, NULL);
    mqtt_set_puback_handler(&client, on_puback, NULL);

    // Connect to the MQTT broker
    mqtt_connect(&client, NULL, NULL, NULL, 0, 0, 0, 0);

    // Main loop
    while (1) {
        // Check for incoming messages
        mqtt_yield(&client, MQTT_INTERVAL);

        // Publish the state
        static bool working = true;
        if (working) {
            publish_state(&client, MQTT_MESSAGE_WORK);
        } else {
            publish_state(&client, MQTT_MESSAGE_NOT_WORK);
        }

        // Toggle the state
        working = !working;

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}