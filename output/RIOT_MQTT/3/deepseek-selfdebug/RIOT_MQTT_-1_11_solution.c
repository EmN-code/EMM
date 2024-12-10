
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_WILL_FLAG 0
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1

static void mqtt_connect(MQTTClient *client)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEAN_SESSION;
    options.willFlag = MQTT_WILL_FLAG;
    options.will.qos = MQTT_WILL_QOS;
    options.will.retained = MQTT_WILL_RETAIN;
    options.will.topicName.cstring = MQTT_WILL_TOPIC;
    options.will.message.cstring = MQTT_WILL_MESSAGE;
    options.username.cstring = MQTT_USERNAME;
    options.password.cstring = MQTT_PASSWORD;

    int rc = MQTTConnect(client, &options);
    if (rc != 0) {
        printf("MQTT connect failed with error code %d\n", rc);
    } else {
        printf("MQTT connected successfully\n");
    }
}

static void mqtt_publish(MQTTClient *client)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    int rc = MQTTPublish(client, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed with error code %d\n", rc);
    } else {
        printf("MQTT message published successfully\n");
    }
}

int main(void)
{
    sock_tcp_t sock;
    MQTTClient client;
    Network network;

    // Initialize the network structure
    network.sock = sock;
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    // Connect to the MQTT broker
    mqtt_connect(&client);

    // Publish a message every 5 seconds
    while (1) {
        mqtt_publish(&client);
        xtimer_sleep(5);
    }

    return 0;
}
