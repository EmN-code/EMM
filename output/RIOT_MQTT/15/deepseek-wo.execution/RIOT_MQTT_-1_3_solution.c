
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <net/sock/udp.h>
#include <time_units.h>
#include <mutex.h>
#include <dht_params.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event/thread.h>
#include <net/gcoap.h>

#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000L

static Network network;
static MQTTClient client;

void publish_message(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    // Initialize network
    NetworkInit(&network);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    // Convert IP address
    struct sockaddr_in remote;
    remote.sin_family = AF_INET;
    remote.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &remote.sin_addr);

    // Connect to the network
    NetworkConnect(&network, SERVER_IP, SERVER_PORT);

    // Connect to MQTT broker
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    MQTTConnect(&client, &options);

    // Publish message every 5 seconds
    while (1) {
        publish_message();
        xtimer_sleep(5);
    }

    // Disconnect from the network
    NetworkDisconnect(&network);

    return 0;
}
