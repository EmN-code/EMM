#include <stdio.h>
#include <string.h>
#include <thread.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/MQTTClient.h>

#define MQTT_SERVER_IP   "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void mqtt_publish(void) {
    MQTTMessage message;
    
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)"work";
    message.payloadlen = strlen("work");

    MQTTPublish(&client, "device/state", &message);
    puts("Published: work");
}

int main(void) {
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_SERVER_IP, MQTT_SERVER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "ESP32Client";

    MQTTConnect(&client, &data);
    puts("MQTT Connected");

    while (1) {
        mqtt_publish();
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}