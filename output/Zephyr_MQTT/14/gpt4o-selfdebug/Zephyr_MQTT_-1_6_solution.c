#include <zephyr/zephyr.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883

#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_STATE_TOPIC "device/state"
#define MQTT_STATE_PAYLOAD_WORK "work"
#define MQTT_STATE_PAYLOAD_NOT_WORK "not work"

#define PUBLISH_INTERVAL K_SECONDS(5)

static struct mqtt_client client;
static struct sockaddr_storage broker;
static struct pollfd fds;
static bool connected;

static void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            if (evt->result == 0) {
                connected = true;
            }
            break;
        case MQTT_EVT_DISCONNECT:
            connected = false;
            break;
        default:
            break;
    }
}

static int init_broker(void) {
    struct sockaddr_in *broker4 = net_sin(&broker);
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker4->sin_addr);
    return 0;
}

static int init_mqtt_client(void) {
    struct mqtt_sec_config *tls_config;

    mqtt_client_init(&client);

    client.broker = &broker;
    client.evt_cb = mqtt_event_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENT_ID;
    client.client_id.size = strlen(MQTT_CLIENT_ID);
    client.protocol_version = MQTT_VERSION_3_1_1;

    tls_config = &client.transport.tls.config;
    tls_config->peer_verify = 0;
    return 0;
}

static int publish_state(void) {
    struct mqtt_publish_param param;

    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_STATE_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_STATE_TOPIC);
    param.message.payload.data = (uint8_t *)(connected ? MQTT_STATE_PAYLOAD_WORK : MQTT_STATE_PAYLOAD_NOT_WORK);
    param.message.payload.len = connected ? strlen(MQTT_STATE_PAYLOAD_WORK) : strlen(MQTT_STATE_PAYLOAD_NOT_WORK);
    param.message_id = sys_rand32_get();
    param.dup_flag = 0;
    param.retain_flag = 0;
    
    return mqtt_publish(&client, &param);
}

void main(void) {
    init_broker();
    init_mqtt_client();

    mqtt_connect(&client);

    fds.fd = client.transport.tcp.sock;
    fds.events = POLLIN;

    while (1) {
        if (connected) {
            publish_state();
        }

        poll(&fds, 1, mqtt_keepalive_time_left(&client));
        mqtt_input(&client);
        mqtt_live(&client);
        
        k_sleep(PUBLISH_INTERVAL);
    }
}