
#include <zephyr/device.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/net/wifi_mgmt.h>
#include <sys/time.h>
#include <zephyr/sys/printk.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/fs/nvs.h>
#include <time.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>

#define MQTT_CLIENT_ID "zephyr_mqtt_client"
#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LENGTH 4
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1

struct mqtt_client client;
uint8_t rx_buffer[100];
uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const c,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("Connected to MQTT broker\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("Disconnected from MQTT broker\n");
            break;
        default:
            break;
    }
}

int main(void)
{
    struct sockaddr_in broker;
    int ret;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);

    client.broker = &broker;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.evt_cb = mqtt_evt_handler;
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = MQTT_CLEAN_SESSION;

    ret = mqtt_connect(&client);
    if (ret) {
        printk("Failed to connect to MQTT broker, error: %d\n", ret);
        return ret;
    }

    while (1) {
        struct mqtt_publish_param param;
        struct mqtt_publish_message message;

        message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
        message.topic.size = strlen(MQTT_TOPIC);
        message.payload.data = (uint8_t *)MQTT_PAYLOAD;
        message.payload.len = MQTT_PAYLOAD_LENGTH;
        message.qos = MQTT_QOS_0_AT_MOST_ONCE;

        param.message = message;
        param.message_id = 0; // Not used for QoS 0
        param.dup_flag = 0;
        param.retain_flag = 0;

        ret = mqtt_publish(&client, &param);
        if (ret) {
            printk("Failed to publish message, error: %d\n", ret);
        } else {
            printk("Message published successfully\n");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
