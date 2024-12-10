
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>
#include <time.h>
#include <sys/time.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_client client;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_publish_param publish_param;
static struct mqtt_topic topic;
static struct mqtt_binstr payload;

void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        printk("WiFi connected\n");
    } else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
        printk("WiFi disconnected\n");
    }
}

void mqtt_event_handler(struct mqtt_client *const c, struct mqtt_evt *evt)
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

int main(void)
{
    struct net_mgmt_event_callback wifi_cb;
    struct net_if *iface;
    struct sockaddr_in broker;
    int ret;

    net_mgmt_init_event_callback(&wifi_cb, wifi_mgmt_event_handler, NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_cb);

    iface = net_if_get_default();
    if (!iface) {
        printk("Failed to get default network interface\n");
        return -1;
    }

    struct wifi_connect_req_params wifi_params = {
        .ssid = "your_wifi_ssid",
        .ssid_length = strlen("your_wifi_ssid"),
        .psk = "your_wifi_password",
        .psk_length = strlen("your_wifi_password"),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(wifi_params));
    if (ret) {
        printk("WiFi connection request failed\n");
        return -1;
    }

    k_sleep(K_SECONDS(10)); // Wait for WiFi to connect

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_client_init(&client);
    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)"zephyr_client";
    client.client_id.size = strlen("zephyr_client");
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client);
    if (ret) {
        printk("MQTT connection failed\n");
        return -1;
    }

    topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    topic.topic.size = strlen(MQTT_TOPIC);
    payload.data = (uint8_t *)MQTT_PAYLOAD;
    payload.size = strlen(MQTT_PAYLOAD);

    publish_param.message.topic = topic;
    publish_param.message.payload = payload;
    publish_param.message_id = 1;
    publish_param.dup_flag = 0;
    publish_param.retain_flag = 0;

    while (1) {
        ret = mqtt_publish(&client, &publish_param);
        if (ret) {
            printk("MQTT publish failed\n");
        } else {
            printk("MQTT publish success\n");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
