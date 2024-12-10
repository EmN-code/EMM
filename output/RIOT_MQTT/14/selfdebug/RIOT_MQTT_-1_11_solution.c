#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqttsn.h>
#include <net/sock.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds
#define DEFAULT_STATE "work"

void publish_state(Network *network, MQTTClient *client) {
    MQTTMessage message;
    char *topic = "device/state";
    char *payload = DEFAULT_STATE;

    message.qos = QOS0;
    message.retained = 0;
    message.payload = payload;
    message.payloadlen = strlen(payload);

    if (MQTTPublish(client, topic, &message) != SUCCESS) {
        printf("Failed to publish message\n");
    } else {
        printf("Successfully published message: %s\n", payload);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to the network\n");
        return -1;
    }

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "esp32_client";

    if (MQTTConnect(&client, &connect_data) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        publish_state(&network, &client);
        xtimer_sleep(PUBLISH_INTERVAL / 1000000U); // Convert microseconds to seconds
    }

    // Normally, you'd never get here in an infinite loop
    return 0;
}