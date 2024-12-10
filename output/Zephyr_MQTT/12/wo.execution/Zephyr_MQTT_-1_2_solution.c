
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/random/random.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/mqtt_sn.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

static int mqtt_connect(int *sock)
{
    struct sockaddr_in broker;
    int ret;

    *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    ret = connect(*sock, (struct sockaddr *)&broker, sizeof(broker));
    if (ret < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(*sock);
        return -1;
    }

    return 0;
}

static int mqtt_publish(int sock)
{
    struct mqtt_publish_param param;
    struct mqtt_client client;
    uint8_t tx_buffer[100];
    uint8_t rx_buffer[100];

    memset(&client, 0, sizeof(client));
    client.broker = (struct sockaddr *)&broker;
    client.broker_len = sizeof(broker);
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.protocol_version = MQTT_VERSION_3_1_1;
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);

    param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.utf8 = (uint8_t *)MQTT_MESSAGE;
    param.message.payload.size = strlen(MQTT_MESSAGE);
    param.message_id = 1U;
    param.dup_flag = 0U;
    param.retain_flag = 0U;
    param.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    int ret = mqtt_publish(&client, &param);
    if (ret < 0) {
        printk("Failed to publish message\n");
        return -1;
    }

    return 0;
}

void main(void)
{
    int sock;

    if (mqtt_connect(&sock) < 0) {
        return;
    }

    while (1) {
        if (mqtt_publish(sock) < 0) {
            break;
        }
        k_sleep(K_SECONDS(5));
    }

    close(sock);
}
