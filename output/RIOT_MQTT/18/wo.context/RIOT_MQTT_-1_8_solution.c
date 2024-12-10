#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/sock/udp.h>
#include <mqttclient.h>  // Ensure this is the last header

#define MQTT_BROKER_ADDRESS "47.102.103.1"
#define MQTT_PORT 1883
#define TOPIC "state"
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL_SEC (5U)

static sock_tcp_ep_t broker_endpoint;
static MQTTClient client;
static Network network;

static void configure_mqtt(void) {
    /* Setup transport layer */
    broker_endpoint.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_ADDRESS, &broker_endpoint.addr);
    broker_endpoint.port = MQTT_PORT;

    /* Initialize the network structure */
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_ADDRESS, MQTT_PORT);

    /* Initialize MQTT client */
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}

static void publish_state(const char* state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    configure_mqtt();
    
    while (1) {
        publish_state(MESSAGE_WORK);
        xtimer_sleep(PUBLISH_INTERVAL_SEC);
        publish_state(MESSAGE_NOT_WORK);
        xtimer_sleep(PUBLISH_INTERVAL_SEC);
    }
    
    /* Disconnect network */
    // Assuming there is a disconnect function like NetworkDisconnect(&network);
    return 0;
}