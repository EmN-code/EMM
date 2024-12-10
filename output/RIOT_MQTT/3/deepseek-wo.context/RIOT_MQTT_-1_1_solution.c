
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void publish_state(void)
{
    static int state = 0;
    const char *message = (state == 0) ? "work" : "not work";
    state = 1 - state; // Toggle state

    // Create a new MQTT message
    MQTTMessage mqtt_message;
    mqtt_message.payload = (void *)message;
    mqtt_message.payloadlen = strlen(message);
    mqtt_message.qos = MQTT_QOS;
    mqtt_message.retained = MQTT_RETAIN;

    // Publish the message
    MQTTPublish(NULL, MQTT_TOPIC, &mqtt_message);
}

int main(void)
{
    // Initialize the network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Initialize the MQTT client
    MQTTClient mqtt_client;
    Network network;
    unsigned char sendbuf[256], readbuf[256];
    MQTTClientInit(&mqtt_client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to the MQTT broker
    IPAddress broker_ip;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_ip);
    NetworkConnect(&network, (char *)&broker_ip, MQTT_BROKER_PORT);
    MQTTPublish(&mqtt_client, MQTT_TOPIC, NULL);

    // Main loop
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
