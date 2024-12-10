
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/wifi.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_context.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/iterable_sections.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_PAYLOAD_WORK "work"
#define MQTT_PAYLOAD_NOT_WORK "not work"
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct sockaddr_storage broker;
static struct k_work_delayable publish_work;

static void wifi_connect(void)
{
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk = WIFI_PSK,
        .psk_length = strlen(WIFI_PSK),
        .channel = WIFI_CHANNEL_ANY,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params))) {
        printk("WiFi connection failed\n");
    } else {
        printk("WiFi connected\n");
    }
}

static void mqtt_connect(void)
{
    int rc;
    struct mqtt_client_config cfg = {
        .client_id = MQTT_CLIENT_ID,
        .keepalive = MQTT_KEEPALIVE,
    };

    mqtt_client_init(&client_ctx);
    client_ctx.broker = &broker;
    client_ctx.client_cfg = &cfg;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    rc = mqtt_connect(&client_ctx);
    if (rc) {
        printk("MQTT connection failed: %d\n", rc);
    } else {
        printk("MQTT connected\n");
    }
}

static void publish_state(struct k_work *work)
{
    static bool state = false;
    int rc;

    state = !state;
    const char *payload = state ? MQTT_PAYLOAD_WORK : MQTT_PAYLOAD_NOT_WORK;

    publish_param.message.topic.qos = MQTT_QOS;
    publish_param.message.topic.topic.utf8 = MQTT_TOPIC;
    publish_param.message.topic.topic.size = strlen(MQTT_TOPIC);
    publish_param.message.payload.data = payload;
    publish_param.message.payload.len = strlen(payload);
    publish_param.dup = false;
    publish_param.retain = MQTT_RETAIN;

    rc = mqtt_publish(&client_ctx, &publish_param);
    if (rc) {
        printk("MQTT publish failed: %d\n", rc);
    } else {
        printk("MQTT published: %s\n", payload);
    }

    k_work_reschedule(&publish_work, PUBLISH_INTERVAL);
}

int main(void)
{
    wifi_connect();
    mqtt_connect();

    k_work_init_delayable(&publish_work, publish_state);
    k_work_reschedule(&publish_work, K_NO_WAIT);

    while (1) {
        k_sleep(K_FOREVER);
    }

    return 0;
}
