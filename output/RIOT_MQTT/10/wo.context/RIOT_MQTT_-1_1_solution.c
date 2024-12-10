#include <string.h>
#include "net/netdev.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "paho_mqtt.h"  // Include the MQTT Paho library
#include "net/emcute.h"
#include "net/netif.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL_MS (5U * 1000U)
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "device/state"
#define MQTT_CONTENT "work"

static bool mqtt_connected = false;

void mqtt_message_handler(MessageData *data) {
    (void)data;
}

static void mqtt_publish(mqtt_client_t *client) {
    if (!mqtt_connected) {
        return;
    }

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MQTT_CONTENT;
    message.payloadlen = strlen(MQTT_CONTENT);

    MQTTClient_publishMessage(client, MQTT_TOPIC, &message);
}

static void mqtt_connect(mqtt_client_t *client, Network *network) {
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    NetworkConnect(network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTClient_connect(client, &connectData);
    mqtt_connected = true;
}

static void *mqtt_thread(void *arg) {
    (void)arg;

    static Network network;
    static mqtt_client_t client;
    static unsigned char sendbuf[MQTT_SENDBUF_SIZE], readbuf[MQTT_READBUF_SIZE];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        if (!mqtt_connected) {
            mqtt_connect(&client, &network);
        }
        
        mqtt_publish(&client);

        vTaskDelay(pdMS_TO_TICKS(MQTT_PUBLISH_INTERVAL_MS));
    }

    return NULL;
}

int main(void) {
    // Initialize network stack or required components here
    mqtt_connected = false;

    xTaskCreate(mqtt_thread, "mqtt", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Start scheduler
    vTaskStartScheduler();

    return 0;
}