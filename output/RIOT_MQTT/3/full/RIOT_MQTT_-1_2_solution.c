#include <string.h>
#include <paho_mqtt.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/af.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define DEVICE_ID "esp32"
#define TOPIC "device/state"

static char mqtt_msg[256];

void publish_state(MQTTClient* client, const char* state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);
    
    int rc = MQTTPublish(client, TOPIC, &message);
    if (rc != SUCCESS) {
        printf("Error while publishing message: %d\n", rc);
    }
}

int mqtt_init_and_connect(MQTTClient* client) {
    Network network;
    MQTTClientInit(client, &network, 1000, malloc(1024), 1024, malloc(1024), 1024);
    
    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != SUCCESS) {
        printf("Error connecting to network: %d\n", rc);
        return rc;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = DEVICE_ID;

    rc = MQTTConnect(client, &data);
    if (rc != SUCCESS) {
        printf("Error connecting to MQTT broker: %d\n", rc);
    }
    return rc;
}

int main(void) {
    MQTTClient client;
    int rc = mqtt_init_and_connect(&client);
    if (rc != SUCCESS) {
        return -1;
    }

    while (1) {
        publish_state(&client, "work");
        xtimer_sleep(5);
        publish_state(&client, "not_work");
        xtimer_sleep(5);
    }
    MQTTDisconnect(&client);
    return 0;
}