
#include <net/sock.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <net/sock/udp.h>
#include <dht_params.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include "MQTTClient.h"  // Ensure this is the last header file to be included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 1
#define MQTT_RETAIN false
#define MQTT_KEEPALIVE 60

static MQTTClient mqttClient;
static Network network;
static unsigned char sendbuf[200];
static unsigned char readbuf[200];

void mqtt_app_start(void) {
    NetworkInit(&network);
    MQTTClientInit(&mqttClient, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = 1;

    rc = MQTTConnect(&mqttClient, &data);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    int rc = MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    mqtt_app_start();

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }

    return 0;
}
