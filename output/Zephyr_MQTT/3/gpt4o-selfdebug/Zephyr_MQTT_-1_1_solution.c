#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param pub_param;

static void prepare_mqtt_client(void) {
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &broker4->sin_addr);

    client.broker = &broker;
    client.client_id.utf8 = (uint8_t *)"zephyr_mqtt_client";
    client.client_id.size = strlen("zephyr_mqtt_client");
    client.protocol_version = MQTT_VERSION_3_1_1;
    mqtt_keepalive_set(&client, 60);
}

static void mqtt_publish_state(const char *state) {
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = (uint8_t *)"state_topic";
    pub_param.message.topic.topic.size = strlen("state_topic");
    pub_param.message.payload.data = (uint8_t *)state;
    pub_param.message.payload.len = strlen(state);
    pub_param.message_id = sys_rand32_get();
    pub_param.dup_flag = 0;
    pub_param.retain_flag = 0;

    mqtt_publish(&client, &pub_param);
}

void main(void) {
    struct mqtt_sec_config tls_config = { 0 };
    prepare_mqtt_client();

    while (1) {
        mqtt_connect(&client);
        k_sleep(K_SECONDS(1));

        if (client.connected) {
            mqtt_publish_state("work");
            k_sleep(PUBLISH_INTERVAL);
            mqtt_publish_state("not work");
            k_sleep(PUBLISH_INTERVAL);
            mqtt_disconnect(&client);
        }
    }
}