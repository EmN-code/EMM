
#include <net/sock.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#include "MQTTClient.h"  // Ensure this is the last header file to be included

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false

void publish_state(MQTTClient client) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = MQTT_PAYLOAD;
    pubmsg.payloadlen = strlen(MQTT_PAYLOAD);
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = MQTT_RETAIN;

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, MQTT_TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
}

int main(void) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, MQTT_BROKER_IP, "RIOT_MQTT_CLIENT", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    while (1) {
        publish_state(client);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
