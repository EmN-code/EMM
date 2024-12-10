
#include <zephyr/device.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/sys/socket.h>
#include <sys/time.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/websocket.h>
#include <zephyr/random/random.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/syscalls/device.h>
#include <time.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/device.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_topic topic;

void publish_state(struct mqtt_client *client) {
    static const char *payload = MQTT_PAYLOAD;
    static const char *topic_name = MQTT_TOPIC;

    publish_param.message.topic.topic.utf8 = (uint8_t *)topic_name;
    publish_param.message.topic.topic.size = strlen(topic_name);
    publish_param.message.payload.data = (uint8_t *)payload;
    publish_param.message.payload.len = strlen(payload);
    publish_param.message_id = 0;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = MQTT_RETAIN;

    mqtt_publish(client, &publish_param);
}

void main(void) {
    struct sockaddr_in broker_addr;
    int ret;

    memset(&client, 0, sizeof(client));
    memset(&broker_addr, 0, sizeof(broker_addr));

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    client.broker = &broker_addr;
    client.client_id.utf8 = (uint8_t *)"esp32_client";
    client.client_id.size = strlen("esp32_client");
    client.password = NULL;
    client.user_name = NULL;
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.clean_session = 1;
    client.keepalive = 60;
    client.will_topic = NULL;
    client.will_msg = NULL;
    client.will_qos = 0;
    client.will_retain = 0;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    mqtt_client_init(&client);

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return;
    }

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }
}
