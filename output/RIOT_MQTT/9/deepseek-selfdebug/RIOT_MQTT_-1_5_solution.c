
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "esp32_client"

#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

static void mqtt_connect(MQTTClient* client, Network* network) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.username.cstring = NULL;
    options.password.cstring = NULL;

    int rc = MQTTConnect(client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
    } else {
        printf("Connected to MQTT broker\n");
    }
}

static void mqtt_publish(MQTTClient* client, const char* topic, const char* message) {
    MQTTMessage pubmsg;
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = MQTT_RETAIN;
    pubmsg.payload = (void*)message;
    pubmsg.payloadlen = strlen(message);

    int rc = MQTTPublish(client, topic, &pubmsg);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published: %s\n", message);
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize network
    Network network;
    NetworkInit(&network);

    // Initialize MQTT client
    MQTTClient client;
    unsigned char sendbuf[256], readbuf[256];
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    mqtt_connect(&client, &network);

    while (1) {
        // Publish message
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);

        // Sleep for 5 seconds
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    }

    return 0;
}
