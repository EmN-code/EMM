#include <zephyr.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <random/rand32.h>
#include <stdio.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

#define STACK_SIZE 1024
#define PRIORITY 7

#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param pub_param;
static struct mqtt_utf8 state_topic = {.utf8 = "state/topic", .size = strlen("state/topic")};
static char payload[] = "work or not work";
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct sockaddr_in local_addr;

void broker_init(void) {
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);
}

void mqtt_evt_handler(struct mqtt_client *const cl, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printf("MQTT client connected!\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printf("MQTT client disconnected!\n");
            break;
        case MQTT_EVT_PUBACK:
            printf("Message published with id: %d\n", evt->param.puback.message_id);
            break;
        default:
            break;
    }
}

void mqtt_connect(void) {
    struct mqtt_sec_config tls_config = {0};

    mqtt_client_init(&client);

    local_addr.sin_family = AF_INET;
    local_addr.sin_port = 0;
    inet_pton(AF_INET, "0.0.0.0", &local_addr.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    connect(client.transport.tcp.sock, (struct sockaddr *)&broker, sizeof(broker));

    mqtt_connect(&client);
}

void mqtt_publish(void) {
    pub_param.message.topic = state_topic;
    pub_param.message.payload.data = payload;
    pub_param.message.payload.len = strlen(payload);
    pub_param.message_id = sys_rand32_get();
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;
    pub_param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    mqtt_publish(&client, &pub_param);
}

void main(void) {
    broker_init();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        k_sleep(PUBLISH_INTERVAL);
    }
}