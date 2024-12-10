#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thread.h"
#include "net/emcute.h"  // Change if necessary as we're not using emcute
#include "paho_mqtt.h"  // Include PAHO MQTT library

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASS "YOUR_WIFI_PASSWORD"
#define PUB_TOPIC "device/state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"
#define PUB_TIMESLOT (5U)

static mqtt_client_t mqtt_client;
static char mqtt_send_buffer[512];
static char mqtt_read_buffer[512];
static msg_t main_msg_queue[8];

void connect_to_wifi(void) {
    // Assume this function connects the ESP32 to the specified Wi-Fi
    // using some existing utility from RIOT for ESP32 Wi-Fi connection.
    // Details depend on specific Wi-Fi stack used in your setup.
}

static void mqtt_on_connect(mqtt_client_t *client) {
    char message[64];
    snprintf(message, sizeof(message), "%s", MSG_WORK);
    mqtt_publish(&mqtt_client, PUB_TOPIC, message, strlen(message), MQTT_QOS_0, false);
}

void *mqtt_thread(void *arg) {
    (void)arg;
    mqtt_client_init(&mqtt_client, mqtt_send_buffer, sizeof(mqtt_send_buffer), 
                     mqtt_read_buffer, sizeof(mqtt_read_buffer), 
                     mqtt_on_connect, NULL);
    mqtt_connect(&mqtt_client, MQTT_BROKER_IP, MQTT_BROKER_PORT, false, "esp32_client", "user", "password", NULL);

    while (1) {
        char message[64];
        snprintf(message, sizeof(message), MSG_WORK);
        mqtt_publish(&mqtt_client, PUB_TOPIC, message, strlen(message), MQTT_QOS_0, false);
        xtimer_sleep(PUB_TIMESLOT);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, sizeof(main_msg_queue) / sizeof(main_msg_queue[0]));

    connect_to_wifi();

    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_thread, NULL, "mqtt_thread");
    return 0;
}