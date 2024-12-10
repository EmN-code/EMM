
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "MQTTClient.h"
#include "network.h"
#include "timer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "esp32/state"
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"

static const char *state_messages[] = { "work", "not work" };

void connect_wifi(void);
void mqtt_pub_task(void *params);

int main(void) {
    // Connect to WiFi
    connect_wifi();

    // Start MQTT publish task
    mqtt_pub_task(NULL);

    return 0;
}

void connect_wifi(void) {
    // Implement WiFi connection logic
    printf("Connecting to WiFi...");
    // esp_wifi_init(); // Example initialization
    // esp_wifi_connect(); // Example connecting
}

void mqtt_pub_task(void *params) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc;
    MQTTMessage message;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    NetworkInit(&network);
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    rc = MQTTConnect(&client, &conn_opts);

    if (rc != SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return;
    }

    while (1) {
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)state_messages[0];
        message.payloadlen = strlen(state_messages[0]);
        MQTTPublish(&client, TOPIC, &message);

        sleep(5);
    }
}