
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/iterable_sections.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"

#define SLEEP_TIME_MS 5000

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param param;

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
        printk("MQTT connected\n");
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT disconnected\n");
        break;
    default:
        break;
    }
}

static int connect_to_wifi(void)
{
    struct net_mgmt_event_callback wifi_mgmt_cb;
    struct wifi_connect_req_params wifi_params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    net_mgmt_init_event_callback(&wifi_mgmt_cb, wifi_mgmt_event_handler,
                                 NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_mgmt_cb);

    return net_mgmt_event_wait_on_iface(net_if_get_default(), NET_EVENT_WIFI_CONNECT_RESULT,
                                        &wifi_params, sizeof(wifi_params), K_FOREVER);
}

static int connect_to_mqtt(void)
{
    struct sockaddr_storage broker;
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);

    mqtt_client_init(&client_ctx);

    client_ctx.broker = (struct sockaddr *)&broker;
    client_ctx.broker_len = sizeof(broker);
    client_ctx.evt_cb = mqtt_evt_handler;
    client_ctx.client_id.utf8 = (uint8_t *)"zephyr_client";
    client_ctx.client_id.size = strlen("zephyr_client");
    client_ctx.protocol_version = MQTT_VERSION_3_1_1;

    return mqtt_connect(&client_ctx);
}

static int publish_state(const char *state)
{
    param.message.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.utf8 = (uint8_t *)state;
    param.message.payload.size = strlen(state);
    param.message.payload_len = strlen(state);
    param.message.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.retained = 0;
    param.dup_flag = 0;

    return mqtt_publish(&client_ctx, &param);
}

int main(void)
{
    int ret;

    printk("Starting MQTT example\n");

    ret = connect_to_wifi();
    if (ret) {
        printk("Failed to connect to WiFi: %d\n", ret);
        return ret;
    }

    ret = connect_to_mqtt();
    if (ret) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        return ret;
    }

    while (1) {
        ret = publish_state("work");
        if (ret) {
            printk("Failed to publish state: %d\n", ret);
        } else {
            printk("State published: work\n");
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));

        ret = publish_state("not work");
        if (ret) {
            printk("Failed to publish state: %d\n", ret);
        } else {
            printk("State published: not work\n");
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
