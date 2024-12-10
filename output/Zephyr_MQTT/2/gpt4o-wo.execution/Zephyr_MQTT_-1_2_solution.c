#include <zephyr/storage/flash_map.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_publish_param pub_param;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static void prepare_mqtt_client(void) {
    mqtt_client_init(&client);

    broker.ss_family = AF_INET;
    net_sin(&broker)->sin_port = htons(MQTT_BROKER_PORT);
    net_sin(&broker)->sin_addr.s_addr = inet_addr(MQTT_BROKER_ADDR);

    client.broker = &broker;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    // Continuing with default client ID and similar default settings...
}

void main(void) {
    prepare_mqtt_client();

    while (mqtt_connect(&client) != 0) {
        printk("Connection to MQTT broker failed. Retrying...");
        k_sleep(K_SECONDS(1));
    }

    pub_param.message.topic.topic.utf8 = MQTT_TOPIC;
    pub_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    pub_param.message.payload.data = "work";
    pub_param.message.payload.len = strlen("work");
    pub_param.message_id = 1;
    pub_param.dup_flag = false;
    pub_param.retain_flag = false;

    while (1) {
        pub_param.message.payload.data = "work";
        mqtt_publish(&client, &pub_param);
        k_sleep(PUBLISH_INTERVAL);

        pub_param.message.payload.data = "not work";
        mqtt_publish(&client, &pub_param);
        k_sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&client);
}