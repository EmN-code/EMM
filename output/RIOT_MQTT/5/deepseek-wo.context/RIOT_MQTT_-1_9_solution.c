
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  /* 5 seconds in microseconds */

static MQTTClient client;
static Network network;

static void connect_to_broker(void)
{
    printf("Connecting to MQTT broker at %s:%d\n", BROKER_IP, BROKER_PORT);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    int rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return;
    }
    printf("Connected to MQTT broker\n");
}

static void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Published message: %s\n", PAYLOAD);
    }
}

int main(void)
{
    printf("Starting MQTT client on ESP32\n");

    /* Initialize the network and MQTT client */
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    /* Connect to the MQTT broker */
    connect_to_broker();

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
