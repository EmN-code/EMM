
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>
#include <timex.h>
#include <thread.h>
#include <msg.h>
#include <xtimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static msg_t msg_queue[8];
static MQTTClient mqtt_client;

void mqtt_publish_state(void) {
    char message[20];
    snprintf(message, sizeof(message), "%s", MQTT_MESSAGE);
    int result = MQTTClient_publish(&mqtt_client, MQTT_TOPIC, strlen(message), message, 0, 0, 0);
    if (result != 0) {
        printf("Failed to publish message: %d\n", result);
    }
}

void mqtt_connect(void) {
    IPAddr broker_ip;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_ip);
    sock_udp_ep_t broker_ep = { .family = AF_INET, .port = MQTT_BROKER_PORT, .addr = { .ipv4 = broker_ip } };

    int result = MQTTClient_connect(&mqtt_client, &broker_ep);
    if (result != 0) {
        printf("Failed to connect to MQTT broker: %d\n", result);
        return;
    }

    printf("Connected to MQTT broker\n");
}

void mqtt_disconnect(void) {
    MQTTClient_disconnect(&mqtt_client);
    printf("Disconnected from MQTT broker\n");
}

void mqtt_thread(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish_state();
        xtimer_usleep(MQTT_MESSAGE_INTERVAL);
    }

    mqtt_disconnect();
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
