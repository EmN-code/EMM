
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"

#define SLEEP_TIME_MS   5000

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb,
                                    uint32_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        printk("WiFi connected\n");
    } else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
        printk("WiFi disconnected\n");
    }
}

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            printk("Failed to connect to MQTT broker: %d\n", evt->result);
        } else {
            printk("Connected to MQTT broker\n");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        printk("Disconnected from MQTT broker\n");
        break;
    default:
        break;
    }
}

static int connect_to_mqtt_broker(void)
{
    struct sockaddr_storage broker;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    int ret;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    ret = inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);
    if (ret < 0) {
        printk("Invalid IP address\n");
        return -1;
    }

    mqtt_client_init(&client_ctx);
    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.broker_len = sizeof(broker);
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = (uint8_t *)"zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.password = NULL;
    client_ctx.user_name = NULL;
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        printk("MQTT connect failed: %d\n", ret);
        return -1;
    }

    return 0;
}

static int publish_state(const char *state)
{
    int ret;

    publish_param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    publish_param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = (uint8_t *)state;
    publish_param.message.payload.len = strlen(state);
    publish_param.message_id = 0;

    ret = mqtt_publish(&client_ctx, &publish_param);
    if (ret) {
        printk("MQTT publish failed: %d\n", ret);
        return -1;
    }

    return 0;
}

int main(void)
{
    struct net_mgmt_event_callback wifi_mgmt_cb;
    struct net_if *iface = net_if_get_default();
    int ret;

    net_mgmt_init_event_callback(&wifi_mgmt_cb, wifi_mgmt_event_handler,
                                 NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_mgmt_cb);

    struct wifi_connect_req_params wifi_params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(wifi_params));
    if (ret) {
        printk("WiFi connection request failed: %d\n", ret);
        return -1;
    }

    k_sleep(K_SECONDS(10)); // Wait for WiFi to connect

    ret = connect_to_mqtt_broker();
    if (ret) {
        return -1;
    }

    while (1) {
        publish_state("work");
        k_sleep(K_MSEC(SLEEP_TIME_MS));
        publish_state("not work");
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}