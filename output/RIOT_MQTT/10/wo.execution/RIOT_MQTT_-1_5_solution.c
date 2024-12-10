#include <sys/socket.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dht.h>
#include <net/ipv6/addr.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <thread.h>
#include <dht_params.h>
#include <esp_wifi.h>
#include <net/emcute.h>
#include <esp_system.h>
#include <event/thread.h>
#include <mma8x5x_params.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>  // Ensure this is included last

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEP_ALIVE 60

#define SLEEP_TIME 5

static MQTTClient client;
static Network network;

void connect_to_broker(void) {
    int rc;
    unsigned char buf[100];    // Buffer for sending MQTT data
    unsigned char readbuf[100];

    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, buf, sizeof(buf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = MQTT_CLIENT_ID;
    connect_data.keepAliveInterval = MQTT_KEEP_ALIVE;
    connect_data.cleansession = 1;

    if ((rc = MQTTConnect(&client, &connect_data)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        abort();
    }
    printf("Connected to MQTT broker!\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    int rc;
    if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
        printf("Error publishing message: %d\n", rc);
    } else {
        printf("Message published: %s\n", MQTT_PAYLOAD);
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        sleep(SLEEP_TIME);
    }
    return 0;
}