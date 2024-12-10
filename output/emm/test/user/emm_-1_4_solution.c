#include "ztimer.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "stdio.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "string.h"
#include "stdlib.h"
#include "msg.h"
#include "net/sock/udp.h"
#include "dht.h"
#include "paho_mqtt.h"

#define MQTT_ADDRESS   "tcp://47.102.103.1:1883"
#define CLIENTID       "ExampleClientID"
#define QOS            1
#define TIMEOUT        10000L
#define DHT_PIN        GPIO_PIN(0, 0)
#define SLEEP_TIME     (60U)

static dht_t dht;
MQTTAsync client;

void connlost(void *context, char *cause) {
    printf("Connection lost, cause: %s\n", cause);
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message) {
    printf("Message arrived: topic=%s, payload=%.*s\n", topicName, message->payloadlen, (char*)message->payload);
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}

void onConnectFailure(void* context, MQTTAsync_failureData* response) {
    printf("Connect failed, rc=%d\n", response ? response->code : 0);
}

void onConnectSuccess(void* context, MQTTAsync_successData* response) {
    printf("Successful connection\n");
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);
    
    dht.params.pin = DHT_PIN;
    if (dht_init(&dht, &dht.params) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    MQTTAsync_create(&client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTAsync_setCallbacks(client, NULL, connlost, msgarrvd, NULL);

    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.onSuccess = onConnectSuccess;
    conn_opts.onFailure = onConnectFailure;
    conn_opts.context = client;

    if (MQTTAsync_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        printf("Failed to start connect, return code\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dht, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
            pubmsg.payload = message;
            pubmsg.payloadlen = strlen(message);
            pubmsg.qos = QOS;
            pubmsg.retained = 0;
            MQTTAsync_deliveryToken token;
            int rc;

            char message[64];
            snprintf(message, sizeof(message), "Temperature: %d, Humidity: %d", temp, hum);

            rc = MQTTAsync_sendMessage(client, TOPIC, &pubmsg, &token);
            if (rc != MQTTCLIENT_SUCCESS)
                printf("Failed to publish message, return code %d\n", rc);
            else
                printf("Message published\n");
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_sleep(SLEEP_TIME);
    }

    MQTTAsync_disconnect(client, NULL);
    MQTTAsync_destroy(&client);

    return 0;
}