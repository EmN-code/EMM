
#include <net/sock.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTClient_publishMessage(client, TOPIC, &message, NULL);
    if (rc != MQTTCLIENT_SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, BROKER_IP, "RIOT_MQTT_Client", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    while (1) {
        publish_state(&client);
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 5 seconds
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return 0;
}
