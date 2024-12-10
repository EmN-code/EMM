#include <stdio.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_ADDR   "47.102.103.1"
#define MQTT_BROKER_PORT   1883
#define PUBLISH_INTERVAL   (5U * US_PER_SEC)

static sock_udp_ep_t broker_endpoint = {
    .family = AF_INET,
    .ipv4 = { .u32 = 0 },
    .port = MQTT_BROKER_PORT
};

int main(void) {
    char *topic = "device/state";
    char *message = "work";
    uint8_t buffer[128];
    MQTTClient client;
    Network network;
    MQTTMessage mqtt_message;

    // Setup UDP endpoint and parse broker IP
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_endpoint.ipv4.u8);

    MQTTClientInit(&client, &network, 1000, buffer, sizeof(buffer), buffer, sizeof(buffer));

    // Connect to MQTT broker
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "riot-client";

    if (MQTTConnect(&client, &connect_data) != SUCCESS) {
        puts("MQTT connection failed");
        return 1;
    }

    puts("MQTT connected");

    // Main loop
    while (1) {
        mqtt_message.qos = QOS0;
        mqtt_message.retained = 0;
        mqtt_message.dup = 0;
        mqtt_message.payload = (void *)message;
        mqtt_message.payloadlen = strlen(message);
        
        if (MQTTPublish(&client, topic, &mqtt_message) != SUCCESS) {
            puts("Failed to publish MQTT message");
        } else {
            puts("MQTT message published");
        }

        // Wait for the next publish
        xtimer_usleep(PUBLISH_INTERVAL);
    }
    
    return 0;
}