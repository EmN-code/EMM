
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"

static void mqtt_event_handler(struct MQTTClient *client, int event, void *data) {
    (void)client;
    (void)data;
    if (event == MQTT_EVENT_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else if (event == MQTT_EVENT_DISCONNECTED) {
        printf("Disconnected from MQTT broker\n");
    }
}

int main(void) {
    sock_tcp_t sock;
    struct sockaddr_in server_addr;
    MQTTClient client;
    MQTTMessage message;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (sock_tcp_create(&sock, NULL, (struct sockaddr *)&server_addr, 0) < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    MQTTClient_init(&client, &sock, mqtt_event_handler, NULL, NULL);

    if (MQTTClient_connect(&client, CLIENT_ID, NULL, NULL, 0, NULL, NULL, 0, 60) != MQTT_OK) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != MQTT_OK) {
            printf("Failed to publish message\n");
        } else {
            printf("Published: %s\n", PAYLOAD);
        }
        xtimer_sleep(5);
    }

    MQTTClient_disconnect(&client, 1000);
    sock_tcp_close(&sock);

    return 0;
}
