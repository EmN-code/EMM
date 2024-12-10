#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "mutex.h"
#include "mqttClient.h"
#include "esp_event.h"
#include "paho_mqtt.h"
#include "time_units.h"
#include "dht_params.h"
#include "dht.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "esp_wifi.h"

#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define MQTT_PUB_TOPIC      "device/state"
#define CLIENT_ID           "esp32device"
#define QOS                 QOS0
#define PUBLISH_INTERVAL    (5U) // in seconds

// Placeholder network read and write functions
int Network_read(Network* n, unsigned char* read_buffer, int len, int timeout) {
    // Implement the function using your specific network APIs
    return 0;
}

int Network_write(Network* n, unsigned char* send_buffer, int len, int timeout) {
    // Implement the function using your specific network APIs
    return 0;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[100], readbuf[100];
    Timer timer;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Initialize network
    NetworkInit(&network);
    network.mqttread = Network_read;
    network.mqttwrite = Network_write;
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Configure connection data
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;

    // Connect to the broker
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("MQTT connect failed\n");
        return -1;
    }
    printf("MQTT connected\n");

    MQTTMessage message;

    // Publish loop
    while (1) {
        // Prepare payload for "work" or "not work"
        static char payload[] = "work"; // or "not work"

        message.qos = QOS;
        message.retained = 0;
        message.payload = (void*)payload;
        message.payloadlen = strlen(payload);

        if (MQTTPublish(&client, MQTT_PUB_TOPIC, &message) != SUCCESS) {
            printf("MQTT publish failed\n");
        } else {
            printf("MQTT message published\n");
        }

        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL);
    }

    return 0;
}