
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/devicetree.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/wifi.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/buf.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/random/random.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/unistd.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_SIZE 4
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1

LOG_MODULE_REGISTER(main);

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("MQTT connect failed %d", evt->result);
        } else {
            LOG_INF("MQTT connected!");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT disconnected!");
        break;
    default:
        break;
    }
}

void publish_state(struct mqtt_client *client)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic = {
        .topic.utf8 = MQTT_TOPIC,
        .topic.size = strlen(MQTT_TOPIC),
    };
    struct mqtt_publish_message message = {
        .topic = &topic,
        .payload.data = MQTT_PAYLOAD,
        .payload.len = MQTT_PAYLOAD_SIZE,
        .qos = MQTT_QOS_1_AT_LEAST_ONCE,
        .retain = 0,
    };

    param.message = message;
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    int ret = mqtt_publish(client, &param);
    if (ret) {
        LOG_ERR("Failed to publish, error: %d", ret);
    } else {
        LOG_INF("State published");
    }
}

int main(void)
{
    struct sockaddr_in broker;
    int sock;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    sock = zsock_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket, error: %d", sock);
        return -1;
    }

    if (zsock_connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        LOG_ERR("Failed to connect to broker");
        zsock_close(sock);
        return -1;
    }

    memset(&client, 0, sizeof(client));
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = MQTT_CLEAN_SESSION;

    mqtt_client_init(&client);

    if (mqtt_connect(&client) != 0) {
        LOG_ERR("MQTT connect failed");
        zsock_close(sock);
        return -1;
    }

    while (1) {
        publish_state(&client);
        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client);
    zsock_close(sock);

    return 0;
}
