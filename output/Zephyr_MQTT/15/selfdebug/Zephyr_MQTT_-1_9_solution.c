
#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_core.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/sys/printk.h>
#include <string.h>

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883

#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

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

int main(void)
{
    struct net_if *iface = net_if_get_default();
    struct net_mgmt_event_callback wifi_mgmt_cb;
    struct mqtt_client_config mqtt_cfg;
    struct sockaddr_in broker;

    // Initialize WiFi
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

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(wifi_params))) {
        printk("WiFi connection request failed\n");
        return -1;
    }

    // Wait for WiFi to connect
    k_sleep(K_SECONDS(10));

    // Initialize MQTT client
    memset(&client_ctx, 0, sizeof(client_ctx));
    memset(&mqtt_cfg, 0, sizeof(mqtt_cfg));

    mqtt_cfg.client_id.utf8 = "zephyr_client";
    mqtt_cfg.client_id.size = strlen(mqtt_cfg.client_id.utf8);
    mqtt_cfg.user_name = NULL;
    mqtt_cfg.password = NULL;
    mqtt_cfg.keepalive = 60;
    mqtt_cfg.clean_session = 1;
    mqtt_cfg.keepalive = 60;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    mqtt_init(&client_ctx, &mqtt_cfg);
    mqtt_connect(&client_ctx, (struct sockaddr *)&broker, sizeof(broker));

    // Publish state every 5 seconds
    while (1) {
        publish_param.message.topic.utf8 = MQTT_TOPIC;
        publish_param.message.topic.size = strlen(MQTT_TOPIC);
        publish_param.message.payload.data = MQTT_PAYLOAD;
        publish_param.message.payload.len = strlen(MQTT_PAYLOAD);
        publish_param.message_id = 0;
        publish_param.dup_flag = 0;
        publish_param.retain_flag = 0;

        mqtt_publish(&client_ctx, &publish_param);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
