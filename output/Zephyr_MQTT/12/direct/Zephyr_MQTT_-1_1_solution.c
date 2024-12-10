
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
    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        LOG_INF("WiFi connected");
    } else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
        LOG_INF("WiFi disconnected");
    }
}

static void mqtt_evt_handler(struct mqtt_client *const client,
                             const struct mqtt_evt *evt)
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

static int connect_to_mqtt_broker(void)
{
    int rc;
    struct sockaddr_in broker;

    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    rc = mqtt_client_init(&client_ctx);
    if (rc) {
        LOG_ERR("Failed to initialize MQTT client, error: %d", rc);
        return rc;
    }

    mqtt_conf.client_id = MQTT_CLIENT_ID;
    mqtt_conf.user_name = NULL;
    mqtt_conf.password = NULL;
    mqtt_conf.keep_alive = 60;
    mqtt_conf.clean_session = true;
    mqtt_conf.will_topic = NULL;
    mqtt_conf.will_msg = NULL;
    mqtt_conf.will_qos = 0;
    mqtt_conf.will_retain = false;

    rc = mqtt_connect(&client_ctx, (struct sockaddr *)&broker, &mqtt_conf,
                      mqtt_evt_handler, K_NO_WAIT);
    if (rc) {
        LOG_ERR("Failed to connect to MQTT broker, error: %d", rc);
        return rc;
    }

    return 0;
}

static int publish_state(const char *state)
{
    int rc;
    struct mqtt_publish_param param;

    topic.utf8 = MQTT_TOPIC;
    topic.size = strlen(MQTT_TOPIC);

    param.message.topic = &topic;
    param.message.payload.data = state;
    param.message.payload.len = strlen(state);
    param.message_id = 0;
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.qos = MQTT_QOS_0_AT_MOST_ONCE;

    rc = mqtt_publish(&client_ctx, &param);
    if (rc) {
        LOG_ERR("Failed to publish message, error: %d", rc);
        return rc;
    }

    return 0;
}

void main(void)
{
    struct net_mgmt_event_callback wifi_mgmt_cb;
    struct net_if *iface;
    int rc;

    net_mgmt_init_event_callback(&wifi_mgmt_cb, wifi_mgmt_event_handler,
                                 NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_mgmt_cb);

    iface = net_if_get_default();
    if (!iface) {
        LOG_ERR("No default network interface");
        return;
    }

    rc = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, WIFI_SSID, strlen(WIFI_SSID), WIFI_PSK, strlen(WIFI_PSK));
    if (rc) {
        LOG_ERR("Failed to connect to WiFi, error: %d", rc);
        return;
    }

    k_sleep(K_SECONDS(10)); // Wait for WiFi to connect

    rc = connect_to_mqtt_broker();
    if (rc) {
        return;
    }

    while (1) {
        rc = publish_state("work");
        if (rc) {
            break;
        }
        k_sleep(K_SECONDS(5));
        rc = publish_state("not work");
        if (rc) {
            break;
        }
        k_sleep(K_SECONDS(5));
    }
}
