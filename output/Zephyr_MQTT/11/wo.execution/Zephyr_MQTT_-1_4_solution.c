
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/kernel_includes.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/net/websocket.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/devicetree.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/buf.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/net_if.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/random/random.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/posix/poll.h>
#include <sys/time.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/unistd.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1

LOG_MODULE_REGISTER(main);

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];

void mqtt_evt_handler(struct mqtt_client *const client,
                      const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result == 0) {
            LOG_INF("Connected to MQTT broker");
        } else {
            LOG_ERR("Failed to connect to MQTT broker: %d", evt->result);
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("Disconnected from MQTT broker");
        break;
    default:
        break;
    }
}

int connect_mqtt(void)
{
    struct sockaddr_in broker;
    int ret;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = "zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;
    client_ctx.clean_session = MQTT_CLEAN_SESSION;
    client_ctx.keepalive = MQTT_KEEPALIVE;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("MQTT connect failed: %d", ret);
        return ret;
    }

    return 0;
}

int publish_state(void)
{
    struct mqtt_publish_param param;
    struct mqtt_topic topic;
    int ret;

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;

    param.message.topic = &topic;
    param.message.payload.data = MQTT_PAYLOAD;
    param.message.payload.len = MQTT_PAYLOAD_LEN;
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;

    ret = mqtt_publish(&client_ctx, &param);
    if (ret) {
        LOG_ERR("MQTT publish failed: %d", ret);
        return ret;
    }

    return 0;
}

void main(void)
{
    int ret;

    ret = connect_mqtt();
    if (ret) {
        return;
    }

    while (1) {
        ret = publish_state();
        if (ret) {
            break;
        }
        k_sleep(K_SECONDS(5));
    }

    mqtt_disconnect(&client_ctx);
}
