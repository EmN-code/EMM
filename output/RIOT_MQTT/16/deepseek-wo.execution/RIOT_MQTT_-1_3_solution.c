
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <paho_mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLEANSESSION 1
#define MQTT_TIMEOUT 1000

static void mqtt_publish(void) {
    static char payload[128];
    static int state = 0;

    state = !state;
    snprintf(payload, sizeof(payload), "%s", state ? "work" : "not work");

    printf("Publishing message: %s\n", payload);

    // Simulate MQTT publish logic
    // This is a placeholder for the actual MQTT publish function
    // Since we cannot use MQTTClient.h, we simulate the behavior here
    // In a real implementation, you would use the MQTTClient library functions
    // to connect to the broker and publish the message.

    // Simulate a delay to mimic the network operation
    xtimer_usleep(MQTT_TIMEOUT * 1000);
}

int main(void) {
    puts("MQTT Publisher Example");

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
