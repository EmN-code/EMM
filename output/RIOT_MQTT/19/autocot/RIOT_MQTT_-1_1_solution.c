#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqtt.h>
#include <net/sock/udp.h>
#include <xtimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "esp32/state"
#define MESSAGE_INTERVAL (5U * US_PER_SEC)

static char send_buf[256];
static char recv_buf[256];

static void message_callback(const mqtt_client_t *client, mqtt_message_data_t *msg_data) {
    (void)client;
    printf("Received message on topic %.*s: %.*s\n",
           (int)msg_data->topic->len, msg_data->topic->s,
           (int)msg_data->message->len, msg_data->message->s);
}

static int publish_state(mqtt_client_t *client, const char *state) {
    mqtt_message_t message;

    message.qos = MQTT_QOS0;
    message.retained = false;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    return mqtt_publish(client, TOPIC, &message);
}

int main(void) {
    mqtt_client_t client;
    mqtt_network_t network;
    mqtt_client_init(&client, &network, NULL, send_buf, sizeof(send_buf), recv_buf, sizeof(recv_buf), message_callback);

    struct sockaddr_in broker_addr;
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };

    if (sock_udp_create(NULL, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    mqtt_connect(&client, CLIENT_ID, &remote);

    char *states[2] = {"work", "not work"};
    int idx = 0;

    while (1) {
        if (publish_state(&client, states[idx]) < 0) {
            puts("Error publishing MQTT message");
        }
        idx = (idx + 1) % 2;
        xtimer_sleep(MESSAGE_INTERVAL / US_PER_SEC);
    }

    return 0;
}