#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_in broker;
static struct mqtt_publish_param pub_param;

static void setup_mqtt_client(void) {
    struct mqtt_sec_config sec_config = {};

    /* MQTT client configuration */
    mqtt_client_init(&client);
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = NULL;  /* Add your event handler here if necessary */
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.rx_buf = NULL;
    client.rx_buf_size = 0;
    client.tx_buf = NULL;
    client.tx_buf_size = 0;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
}

static void mqtt_publish_message(struct mqtt_client *client) {
    const char *state_msg = "work or not work";
    pub_param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    pub_param.message.topic.topic.utf8 = (uint8_t *)"zephyr/state";
    pub_param.message.topic.topic.size = strlen("zephyr/state");
    pub_param.message.payload.data = (uint8_t *)state_msg;
    pub_param.message.payload.len = strlen(state_msg);
    pub_param.message_id = sys_rand32_get();
    pub_param.dup_flag = 0U;
    pub_param.retain_flag = 0U;

    mqtt_publish(client, &pub_param);
}

void main(void) {
    int rc;

    setup_mqtt_client();

    rc = mqtt_connect(&client);
    if (rc != 0) {
        printk("MQTT connection failed: %d\n", rc);
        return;
    }

    while (1) {
        mqtt_publish_message(&client);
        k_sleep(PUBLISH_INTERVAL);
    }
}