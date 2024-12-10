#include <zephyr/types.h>
#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <net/net_ip.h>
#include <net/net_mgmt.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "esp32_client"
#define MQTT_TOPIC "state_topic"
#define MQTT_MSG_WORK "work"
#define MQTT_MSG_NOT_WORK "not work"

#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static char payload[128];

static struct mqtt_utf8 client_id = {
    .utf8 = (uint8_t *)MQTT_CLIENTID,
    .size = sizeof(MQTT_CLIENTID) - 1
};

static void connect_to_broker(void) {
    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.client_id = client_id;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;

    mqtt_connect(&client);
}

void main(void) {
    connect_to_broker();

    while (1) {
        snprintf(payload, sizeof(payload), MQTT_MSG_WORK);

        struct mqtt_publish_param param;
        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        param.message.topic.topic.size = strlen(MQTT_TOPIC);
        param.message.payload.data = payload;
        param.message.payload.len = strlen(payload);
        param.message_id = 1;
        param.dup_flag = 0;
        param.retain_flag = 0;

        mqtt_publish(&client, &param);

        k_sleep(PUBLISH_INTERVAL);
    }
}