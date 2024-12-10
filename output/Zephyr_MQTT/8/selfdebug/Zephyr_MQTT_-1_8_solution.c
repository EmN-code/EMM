
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>
#include <mqtt.h>
#include <net/net_context.h>
#include <net/net_mgmt.h>
#include <net/net_if.h>
#include <net/wifi_mgmt.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(main);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        LOG_INF("WiFi connected");
    } else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
        LOG_INF("WiFi disconnected");
    }
}

void mqtt_evt_handler(struct mqtt_client *const client, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            LOG_INF("MQTT connected");
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("MQTT disconnected");
            break;
        default:
            break;
    }
}

void main(void)
{
    struct net_mgmt_event_callback wifi_cb;
    struct sockaddr_in broker;
    int ret;

    net_mgmt_init_event_callback(&wifi_cb, wifi_mgmt_event_handler, NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_cb);

    // Initialize MQTT client
    memset(&client_ctx, 0, sizeof(client_ctx));
    client_ctx.broker = &broker;
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    // Set up broker address
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    // Connect to MQTT broker
    ret = mqtt_connect(&client_ctx);
    if (ret) {
        LOG_ERR("Failed to connect to MQTT broker: %d", ret);
        return;
    }

    // Publish state every 5 seconds
    while (1) {
        k_sleep(K_SECONDS(5));

        publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
        publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
        publish_param.message.payload.data = MQTT_MESSAGE;
        publish_param.message.payload.len = strlen(MQTT_MESSAGE);
        publish_param.message.qos = MQTT_QOS_0_AT_MOST_ONCE;
        publish_param.message.retained = 0;
        publish_param.dup_flag = 0;
        publish_param.retain_flag = 0;

        ret = mqtt_publish(&client_ctx, &publish_param);
        if (ret) {
            LOG_ERR("Failed to publish message: %d", ret);
        } else {
            LOG_INF("Message published");
        }
    }
}
