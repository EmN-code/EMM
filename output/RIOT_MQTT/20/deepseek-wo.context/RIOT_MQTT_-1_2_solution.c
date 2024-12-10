
#include <stdio.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define INTERVAL_MS 5000000

static MQTTClient mqtt_client;
static Network mqtt_network;

void mqtt_connect(void) {
    int rc;
    uint8_t sendbuf[200], readbuf[200];

    NetworkInit(&mqtt_network);
    rc = NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connection failed\n");
        return;
    }

    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.willFlag = 0;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_client";
    data.username.cstring = NULL;
    data.password.cstring = NULL;
    data.keepAliveInterval = 10;
    data.cleansession = 1;

    printf("Connecting to %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);
    rc = MQTTConnect(&mqtt_client, &data);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return;
    }
    printf("Connected to MQTT broker\n");
}

void mqtt_publish(void) {
    int rc;
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = MQTT_MESSAGE_LEN;

    rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed\n");
    } else {
        printf("Published message: %s\n", MQTT_MESSAGE);
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    mqtt_connect();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
        mqtt_publish();
    }

    return 0;
}
