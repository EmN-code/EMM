#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/af.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5LU * US_PER_SEC)

static char mqtt_client_buffer[128];
static uint8_t mqtt_buffer_send[128];
static uint8_t mqtt_buffer_recv[128];

static MQTTClient client;
static Network network;

static const char* topic = "device/state";
static const char* message_working = "work";
static const char* message_not_working = "not work";

void mqtt_connect(void) {
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;    
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_mqtt_client";

    if (MQTTConnect(&client, &connectData)) {
        printf("MQTT connect failed\n");
        return;
    }

    printf("MQTT connected\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS1;
    message.retained = 0;
    message.payload = (void*) message_working;
    message.payloadlen = strlen(message_working);

    MQTTPublish(&client, topic, &message);
    printf("Published: %s\n", (char*)message.payload);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, mqtt_buffer_send, sizeof(mqtt_buffer_send), mqtt_buffer_recv, sizeof(mqtt_buffer_recv));

    ipv4_addr_t addr;
    ipv4_addr_from_str(&addr, SERVER_IP);

    if (network.connect(&network, &addr, SERVER_PORT) < 0) {
        printf("Network connection failed\n");
        return 1;
    }

    mqtt_connect();

    while (1) {
        publish_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}