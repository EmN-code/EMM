#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/random/random.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/init.h>
#include <time.h>
#include <sys/time.h>

#define MQTT_BROKER_IP_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "zephyr_mqtt_client"
#define TOPIC_STATE "device/state"
#define STATE_PAYLOAD_WORK "work"
#define STATE_PAYLOAD_NOT_WORK "not work"
#define PUBLISH_INTERVAL 5

static void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        printk("MQTT client connected!\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected!\n");
        break;
    default:
        printk("Unhandled MQTT event: %d\n", evt->type);
        break;
    }
}

int main(void)
{
    struct mqtt_client client;
    struct sockaddr_in broker;
    uint8_t rx_buffer[128];
    uint8_t tx_buffer[128];

    memset(&client, 0x00, sizeof(client));
    mqtt_client_init(&client);

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    zsock_inet_pton(AF_INET, MQTT_BROKER_IP_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    if (mqtt_connect(&client)) {
        printk("MQTT connect failed\n");
        return -1;
    }

    while (1) {
        struct mqtt_publish_param param;
        k_sleep(K_SECONDS(PUBLISH_INTERVAL));

        memset(&param, 0, sizeof(param));
        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.message.topic.topic.utf8 = TOPIC_STATE;
        param.message.topic.topic.size = strlen(TOPIC_STATE);
        param.message.payload.data = (state % 2 == 0) ? STATE_PAYLOAD_WORK : STATE_PAYLOAD_NOT_WORK;
        param.message.payload.len = strlen(param.message.payload.data);

        if (mqtt_publish(&client, &param)) {
            printk("MQTT publish failed\n");
        }
    }

    mqtt_disconnect(&client);

    return 0;
}
