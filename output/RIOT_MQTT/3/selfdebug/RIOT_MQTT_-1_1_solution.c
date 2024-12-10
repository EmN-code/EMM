#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/MQTTClient.h>
#include <esp_wifi.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define PUBLISH_INTERVAL    5 /* seconds */
#define WORK_STATE          "work"
#define NOT_WORK_STATE      "not work"

/* Function to connect to WiFi (SSID and PASSWORD are placeholders) */
void connect_to_wifi(void) {
    esp_wifi_init(NULL);
    esp_wifi_set_mode(WIFI_MODE_STA);
    wifi_config_t cfg = {
        .sta = {
            .ssid = "your_ssid",         // Replace with your SSID
            .password = "your_password"  // Replace with your password
        }
    };
    esp_wifi_set_config(ESP_IF_WIFI_STA, &cfg);
    esp_wifi_start();
    esp_wifi_connect();
}

/* Function called to publish messages to MQTT Broker */
static void publish(void) {
    static int state = 0;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[80], readbuf[80];
    char *state_message = state ? WORK_STATE : NOT_WORK_STATE;

    /* Set up network and MQTT client */
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    MQTTConnect(&client, NULL);

    /* Create and publish message */
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)state_message;
    message.payloadlen = strlen(state_message);
    MQTTPublish(&client, "device/state", &message);

    /* Toggle state for next message */
    state = !state;
}

int main(void) {
    puts("MQTT example for RIOT
");

    /* Connect to WiFi */
    connect_to_wifi();

    /* Check connection and setup publish every 5 seconds */
    while (1) {
        publish();
        ztimer_sleep(ZTIMER_SEC, PUBLISH_INTERVAL);
    }

    return 0;
}