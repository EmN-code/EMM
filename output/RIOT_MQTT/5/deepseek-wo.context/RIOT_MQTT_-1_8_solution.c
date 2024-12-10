
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient mqtt_client;
static Network mqtt_network;

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&mqtt_client, TOPIC, &message);
}

static void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("MQTT connect failed\n");
        return;
    }

    printf("MQTT connected\n");
}

static void mqtt_init(void)
{
    MQTTClientInit(&mqtt_client, &mqtt_network, 1000, NULL, 0, NULL, 0);

    int rc = NetworkConnect(&mqtt_network, SERVER_IP, SERVER_PORT);
    if (rc != 0) {
        printf("Network connect failed\n");
        return;
    }

    printf("Network connected\n");

    mqtt_connect();
}

int main(void)
{
    mqtt_init();

    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
