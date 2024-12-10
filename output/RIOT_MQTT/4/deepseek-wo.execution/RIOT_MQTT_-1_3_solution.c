
#include <net/sock.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000L

static void messageArrived(MessageData* data) {
    (void)data;
}

int main(void) {
    Network network;
    MQTTClient client;
    int rc;

    // Initialize the network structure
    NetworkInit(&network);

    // Connect to the MQTT broker
    rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    // Set the message arrived callback
    client.messageArrived = messageArrived;

    // Connect to the MQTT broker
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        MQTTPublish(&client, TOPIC, &message);

        xtimer_sleep(5);
    }

    // Disconnect from the MQTT broker
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
