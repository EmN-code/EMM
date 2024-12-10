
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"

static struct mqtt_client client_ctx;
static uint8_t rx_buffer[100];
static uint8_t tx_buffer[100];
static struct mqtt_utf8 topic;
static struct mqtt_utf8 payload;

void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        printk("WiFi connected\n");
    } else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
        printk("WiFi disconnected\n");
    }
}

void mqtt_event_handler(struct mqtt_client *const client, struct mqtt_evt *evt)
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
    struct sockaddr_in broker;
    int sock;
    int ret;

    net_mgmt_init_event_callback(&wifi_cb, wifi_mgmt_event_handler, NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_cb);

    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params wifi_params = {
        .ssid = "your_wifi_ssid",
        .ssid_length = strlen("your_wifi_ssid"),
        .psk = "your_wifi_password",
        .psk_length = strlen("your_wifi_password"),
        .channel = 0,
        .security = WIFI_SECURITY_TYPE_PSK,
    };

    ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(struct wifi_connect_req_params));
    if (ret) {
        printk("WiFi connection failed: %d\n", ret);
        return -1;
    }

    k_sleep(K_SECONDS(10)); // Wait for WiFi to connect

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", sock);
        return -1;
    }

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker.sin_addr);

    ret = connect(sock, (struct sockaddr *)&broker, sizeof(broker));
    if (ret < 0) {
        printk("Failed to connect to MQTT broker: %d\n", ret);
        close(sock);
        return -1;
    }

    mqtt_client_init(&client_ctx);
    client_ctx.broker = &broker;
    client_ctx.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client_ctx.transport.tcp.sock = sock;
    client_ctx.evt_cb = mqtt_event_handler;
    client_ctx.rx_buf = rx_buffer;
    client_ctx.rx_buf_size = sizeof(rx_buffer);
    client_ctx.tx_buf = tx_buffer;
    client_ctx.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client_ctx);
    if (ret) {
        printk("MQTT connect failed: %d\n", ret);
        close(sock);
        return -1;
    }

    topic.utf8 = MQTT_TOPIC;
    topic.size = strlen(MQTT_TOPIC);
    payload.utf8 = MQTT_PAYLOAD;
    payload.size = strlen(MQTT_PAYLOAD);

    struct mqtt_publish_param param = {
        .message = {
            .topic = {
                .topic = topic,
            },
            .payload = {
                .data = payload,
            },
        },
        .message_id = 1,
        .dup_flag = 0,
        .retain_flag = 0,
    };

    while (1) {
        ret = mqtt_publish(&client_ctx, &param);
        if (ret) {
            printk("MQTT publish failed: %d\n", ret);
        } else {
            printk("MQTT message published\n");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
