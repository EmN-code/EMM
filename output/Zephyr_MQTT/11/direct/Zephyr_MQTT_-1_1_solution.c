
#include <zephyr.h>
#include <net/socket.h>
#include <logging/log.h>
#include <net/mqtt.h>
#include <net/net_context.h>
#include <net/net_mgmt.h>
#include <net/net_if.h>
#include <net/wifi_mgmt.h>

#define LOG_LEVEL LOG_LEVEL_DBG
LOG_MODULE_REGISTER(mqtt_example);

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "zephyr_esp32"

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PSK "your_wifi_password"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_utf8 topic;
static struct mqtt_client_config mqtt_conf;

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb,
                                    uint32_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event != NET_EVENT_WIFI_CONNECT_RESULT) {
        return;
    }

    if (cb->info->status == 0) {
        LOG_INF("Connected to WiFi");
    } else {
        LOG_ERR("Failed to connect to WiFi");
    }
}

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            LOG_ERR("MQTT connect failed %d", evt->result);
        } else {
            LOG_INF("MQTT connected");
        }
        break;
    case MQTT_EVT_DISCONNECT:
        LOG_INF("MQTT disconnected");
        break;
    default:
        break;
    }
}

static void connect_to_wifi(void)
{
    struct net_if *iface = net_if_get_default();
    struct net_mgmt_event_callback wifi_mgmt_cb;
    struct wifi_connect_req_params wifi_params;

    net_mgmt_init_event_callback(&wifi_mgmt_cb, wifi_mgmt_event_handler,
                                 NET_EVENT_WIFI_CONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_mgmt_cb);

    wifi_params.ssid = WIFI_SSID;
    wifi_params.ssid_length = strlen(WIFI_SSID);
    wifi_params.psk = WIFI_PSK;
    wifi_params.psk_length = strlen(WIFI_PSK);
    wifi_params.channel = WIFI_CHANNEL_ANY;
    wifi_params.security = WIFI_SECURITY_TYPE_PSK;

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(wifi_params))) {
        LOG_ERR("WiFi connection request failed");
    }
}

static void connect_to_mqtt(void)
{
    int rc;
    struct sockaddr_in broker;

    memset(&client_ctx, 0, sizeof(client_ctx));
    memset(&mqtt_conf, 0, sizeof(mqtt_conf));

    mqtt_conf.client_id = MQTT_CLIENT_ID;
    mqtt_conf.user_name = NULL;
    mqtt_conf.password = NULL;
    mqtt_conf.keepalive = 60;
    mqtt_conf.clean_session = true;
    mqtt_conf.resv = 0;

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    rc = mqtt_client_init(&client_ctx, &mqtt_conf, rx_buffer, sizeof(rx_buffer),
                          tx_buffer, sizeof(tx_buffer), mqtt_evt_handler);
    if (rc != 0) {
        LOG_ERR("Failed to initialize MQTT client: %d", rc);
        return;
    }

    rc = mqtt_connect(&client_ctx, (struct sockaddr *)&broker, NULL, NULL, NULL);
    if (rc != 0) {
        LOG_ERR("MQTT connect failed: %d", rc);
    }
}

static void publish_state(void)
{
    int rc;
    static bool state = false;
    const char *state_str = state ? "work" : "not work";

    topic.utf8 = MQTT_TOPIC;
    topic.size = strlen(MQTT_TOPIC);

    rc = mqtt_publish(&client_ctx, &topic, state_str, strlen(state_str), MQTT_QOS_0_AT_MOST_ONCE, false);
    if (rc != 0) {
        LOG_ERR("MQTT publish failed: %d", rc);
    } else {
        LOG_INF("Published state: %s", state_str);
    }

    state = !state;
}

void main(void)
{
    connect_to_wifi();
    k_sleep(K_SECONDS(10)); // Wait for WiFi connection
    connect_to_mqtt();

    while (1) {
        k_sleep(K_SECONDS(5));
        publish_state();
    }
}
