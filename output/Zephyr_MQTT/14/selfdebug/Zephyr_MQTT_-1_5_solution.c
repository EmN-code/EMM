
#include <zephyr.h>
#include <net/socket.h>
#include <net/mqtt.h>
#include <logging/log.h>
#include <string.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1
#define MQTT_QOS 0
#define MQTT_RETAIN 0

LOG_MODULE_REGISTER(main);

struct mqtt_client client;
struct mqtt_connect_param conn_params;
struct mqtt_publish_param pub_params;
struct mqtt_topic topic;
struct mqtt_utf8 client_id;
struct mqtt_transport transport;

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                LOG_INF("Connected to MQTT broker");
                pub_params.message.topic = &topic;
                pub_params.message.payload.data = MQTT_PAYLOAD;
                pub_params.message.payload.len = MQTT_PAYLOAD_LEN;
                pub_params.message_id = 0;
                pub_params.dup_flag = 0;
                pub_params.retain_flag = MQTT_RETAIN;
                mqtt_publish(&client, &pub_params);
            } else {
                LOG_ERR("Failed to connect to MQTT broker: %d", evt->result);
            }
            break;
        case MQTT_EVT_DISCONNECT:
            LOG_INF("Disconnected from MQTT broker");
            break;
        default:
            break;
    }
}

void main(void)
{
    int sock;
    struct sockaddr_in broker;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        LOG_ERR("Failed to create socket: %d", sock);
        return;
    }

    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        LOG_ERR("Failed to connect to MQTT broker");
        close(sock);
        return;
    }

    memset(&client, 0, sizeof(client));
    client.transport.type = MQTT_TRANSPORT_NON_SECURE;
    client.transport.tcp.sock = sock;
    client.client_id.utf8 = "zephyr_client";
    client.client_id.size = strlen(client.client_id.utf8);
    client.keepalive = MQTT_KEEPALIVE;
    client.clean_session = MQTT_CLEAN_SESSION;
    client.evt_cb = mqtt_evt_handler;

    topic.topic.utf8 = MQTT_TOPIC;
    topic.topic.size = strlen(topic.topic.utf8);
    topic.qos = MQTT_QOS;

    conn_params.client_id = &client.client_id;
    conn_params.user_name = NULL;
    conn_params.password = NULL;
    conn_params.keepalive = MQTT_KEEPALIVE;
    conn_params.clean_session = MQTT_CLEAN_SESSION;
    conn_params.will_topic = NULL;
    conn_params.will_message = NULL;
    conn_params.will_qos = 0;
    conn_params.will_retain = 0;

    mqtt_connect(&client, &conn_params);

    while (1) {
        k_sleep(K_SECONDS(5));
        mqtt_publish(&client, &pub_params);
    }
}
