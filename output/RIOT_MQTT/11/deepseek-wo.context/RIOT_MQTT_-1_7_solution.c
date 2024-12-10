
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_LARGE)

static void mqtt_publish_state(void)
{
    static MQTTClient client;
    static Network network;
    static unsigned char sendbuf[256];
    static unsigned char readbuf[256];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    // Publish the state message
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Message published: %s\n", MQTT_MESSAGE);
    }

    // Disconnect from the MQTT broker
    MQTTDisconnect(&client);
}

int main(void)
{
    // Initialize the network
    Network network;
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    // Create a thread to handle MQTT publishing
    thread_create(NULL, MQTT_THREAD_STACKSIZE, MQTT_THREAD_PRIORITY, 0, mqtt_publish_state, NULL, "mqtt_thread");

    // Main loop
    while (1) {
        xtimer_sleep(5);
        mqtt_publish_state();
    }

    return 0;
}
