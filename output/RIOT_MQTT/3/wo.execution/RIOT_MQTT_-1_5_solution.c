#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "mutex.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "dht_params.h"
#include "dht.h"
#include "assert.h"
#include "MQTTClient.h" // Keep this as the last header due to its dependency.

#define BROKER_IP      "47.102.103.1"
#define BROKER_PORT    1883
#define CLIENT_ID      "esp32_client"
#define TOPIC          "state"
#define MESSAGE        "work or not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

void mqtt_publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MESSAGE;
    message.payloadlen = strlen(MESSAGE);

    if (MQTTPublish(client, TOPIC, &message) != SUCCESS) {
        printf("Error publishing to MQTT broker\n");
    }
}

int main(void) {
    Network network;

    // Initialize network
    NetworkInit(&network);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to network\n");
        return -1;
    }

    unsigned char sendbuf[80], readbuf[80];
    MQTTClient client;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish_state(&client);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}