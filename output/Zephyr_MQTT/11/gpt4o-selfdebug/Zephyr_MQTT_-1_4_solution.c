#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <random/rand32.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

static struct mqtt_client client;
static struct sockaddr_storage broker;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static char payload[] = "work or not work";
static struct mqtt_publish_param pub_param;

void publish_state(struct k_work *item) {
    pub_param.message.payload.data = payload;
    pub_param.message.payload.len = strlen(payload);
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = "state_topic";
    pub_param.message.topic.topic.size = strlen("state_topic");
    pub_param.message_id = sys_rand32_get();

    mqtt_publish(&client, &pub_param);
}

static struct k_work_delayable work;

void main(void) {
    broker.sa_family = AF_INET;
    net_sin(&broker)->sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &net_sin(&broker)->sin_addr);
 
    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = NULL;
    client.client_id.utf8 = "zephyr_pub";
    client.client_id.size = strlen("zephyr_pub");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    if (mqtt_connect(&client) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    k_work_init_delayable(&work, publish_state);
    k_work_reschedule(&work, K_SECONDS(5));

    while (true) {
        mqtt_input(&client);
    }
}