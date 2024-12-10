
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network mqtt_network;

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

static void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return;
    }

    printf("MQTT connected\n");
}

static void mqtt_init(void)
{
    NetworkInit(&mqtt_network);
    int rc = NetworkConnect(&mqtt_network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Network connect failed\n");
        return;
    }

    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, NULL, 0, NULL, 0);
    mqtt_connect();
}

int main(void)
{
    mqtt_init();

    while (1) {
        mqtt_publish();
        xtimer_usleep(MQTT_INTERVAL);
    }

    return 0;
}
