
#include <net/ipv4/addr.h>
#include <event/thread.h>
#include <time_units.h>
#include <ztimer.h>
#include <esp_system.h>
#include <dht_params.h>
#include <stdio.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <net/gcoap.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000

static Network network;
static MQTTClient client;

void connect_to_broker(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    int rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
    } else {
        printf("Connected to MQTT broker\n");
    }
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    uint32_t ip;
    inet_pton(AF_INET, SERVER_IP, &ip);
    network.my_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = ip;

    if (connect(network.my_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Failed to connect to server\n");
        return -1;
    }

    connect_to_broker();

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    MQTTClose(&client);
    close(network.my_socket);

    return 0;
}
