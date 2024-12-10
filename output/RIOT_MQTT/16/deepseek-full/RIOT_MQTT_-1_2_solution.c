
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <evtimer_mbox.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <MQTTClient.h>
#include <ztimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 20
#define MQTT_QOS 1
#define MQTT_RETAIN false
#define MQTT_CLEAN_SESSION true
#define MQTT_TIMEOUT 10000

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed with return code %d\n", rc);
    } else {
        printf("MQTT connected\n");
    }
}

void mqtt_publish(const char* payload) {
    int rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, payload);
    if (rc != 0) {
        printf("MQTT publish failed with return code %d\n", rc);
    } else {
        printf("MQTT published: %s\n", payload);
    }
}

void mqtt_disconnect(void) {
    int rc = MQTTDisconnect(&mqtt_client);
    if (rc != 0) {
        printf("MQTT disconnect failed with return code %d\n", rc);
    } else {
        printf("MQTT disconnected\n");
    }
}

void mqtt_network_init(void) {
    NetworkInit(&mqtt_network);
    int rc = NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed with return code %d\n", rc);
    } else {
        printf("Network connected\n");
    }
}

void mqtt_client_init(void) {
    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}

void publish_state(void) {
    const char* state = "work"; // or "not work" based on your logic
    mqtt_publish(state);
}

int main(void) {
    mqtt_network_init();
    mqtt_client_init();
    mqtt_connect();

    while (1) {
        publish_state();
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    mqtt_disconnect();
    return 0;
}
