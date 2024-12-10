#include <xtimer.h>
#include <net/mqttsn.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <msg.h>
#include <mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "esp32/state"
#define NODE_ID "esp32-node"
#define PUB_INTERVAL (5U * 1000000U) // 5 seconds in micro seconds

static mqtt_client_t client;
static mqtt_broker_handle_t broker;
static emcute_topic_t topic;

void configure_mqtt(void) {
    mqtt_init(&client, &broker);
    broker.port = MQTT_BROKER_PORT;
    broker.address = MQTT_BROKER_IP;
    topic.name = MQTT_TOPIC;
}

void publish_state(const char *state) {
    mqtt_message_t message;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    message.qos = QOS0;
    message.retained = 0;

    mqtt_publish(&client, &topic, &message);
    printf("Published: %s to topic: %s\n", state, MQTT_TOPIC);
}

int main(void) {
    printf("Starting MQTT Client\n");
    configure_mqtt();

    mqtt_establish_connection(&client, &broker);

    while (1) {
        const char *state = "work"; // Placeholder for actual state logic
        publish_state(state);
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}