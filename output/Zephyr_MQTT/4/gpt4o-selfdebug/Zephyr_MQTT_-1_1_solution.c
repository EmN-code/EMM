#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <net/wifi_mgmt.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct mqtt_utf8 topic = MQTT_UTF8_LITERAL("state");
static struct mqtt_utf8 payload_work = MQTT_UTF8_LITERAL("work");
static struct mqtt_utf8 payload_not_work = MQTT_UTF8_LITERAL("not work");
static struct k_work_delayable publish_work;

static void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            printk("MQTT connect failed: %d
", evt->result);
            return;
        }
        printk("MQTT client connected!
");
        k_work_reschedule(&publish_work, PUBLISH_INTERVAL);
        break;
    case MQTT_EVT_DISCONNECT:
        printk("MQTT client disconnected: %d
", evt->result);
        break;
    default:
        break;
    }
}

static void publish_state(struct k_work *work)
{
    static bool work_state = true;
    struct mqtt_publish_param param;

    param.message.topic = topic;
    param.message.payload.data = work_state ? payload_work.utf8 : payload_not_work.utf8;
    param.message.payload.len = work_state ? payload_work.size : payload_not_work.size;
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;
    param.qos = MQTT_QOS_0_AT_MOST_ONCE;

    mqtt_publish(&client, &param);
    work_state = !work_state;
    k_work_reschedule(&publish_work, PUBLISH_INTERVAL);
}

static void wifi_connect_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event != NET_EVENT_WIFI_CONNECT_RESULT) return;
    struct wifi_connect_req_params params;
    params.ssid = "YOUR_SSID";
    params.ssid_length = strlen(params.ssid);
    params.psk = "YOUR_PASSWORD";
    params.psk_length = strlen(params.psk);
    params.channel = 0;
    params.security = WIFI_SECURITY_TYPE_PSK;
    net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
}

static void configure_mqtt()
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)mqtt_client_id;
    client.client_id.size = strlen(mqtt_client_id);
    mqtt_connect(&client);
}

void main(void)
{
    net_mgmt_init_event_callback(&wifi_connect_cb, wifi_connect_handler, NET_EVENT_WIFI_CONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_connect_cb);
    net_if_up(net_if_get_default());

    configure_mqtt();
    k_work_init_delayable(&publish_work, publish_state);
}