#include <zephyr/sys/iterable_sections.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "zephyr_client"
#define TOPIC "zephyr/esp32/state"
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"

#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static uint8_t mqtt_buffer[128];

static void prepare_mqtt_client(void) {
    mqtt_client_init(&client);
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);
    client.broker = &broker;
    client.client_id.utf8 = (uint8_t *)CLIENT_ID;
    client.client_id.size = strlen(CLIENT_ID);
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = mqtt_buffer;
    client.rx_buf_size = sizeof(mqtt_buffer);
    client.tx_buf = mqtt_buffer;
    client.tx_buf_size = sizeof(mqtt_buffer);
    client.transport.type = MQTT_TRANSPORT_TCP;
}

static void publish_state(bool work_state) {
    struct mqtt_publish_param param;
    param.message.topic.topic.utf8 = (uint8_t *)TOPIC;
    param.message.topic.topic.size = strlen(TOPIC);
    param.message.payload.data = (work_state ? STATE_WORK : STATE_NOT_WORK);
    param.message.payload.len = strlen(work_state ? STATE_WORK : STATE_NOT_WORK);
    param.message_id = 1;
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    mqtt_publish(&client, &param);
}

void main(void) {
    prepare_mqtt_client();
    mqtt_connect(&client);
    bool work_state = true;
    while (1) {
        publish_state(work_state);
        work_state = !work_state;
        k_sleep(PUBLISH_INTERVAL);
    }
}