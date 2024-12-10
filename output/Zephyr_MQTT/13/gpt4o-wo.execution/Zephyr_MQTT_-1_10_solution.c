#include <zephyr/posix/sys/socket.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/kernel.h>
#include <zephyr/pm/state.h>
#include <zephyr/random/random.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/net/buf.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/unistd.h>
#include <sys/time.h>
#include <time.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define KEEP_ALIVE_INTERVAL 60
#define MQTT_TOPIC "esp32/state"
#define MSG_CONTENT "work or not work"
#define PUBLISH_INTERVAL 5000

void mqtt_event_handler(struct mqtt_client *client, const struct mqtt_evt *evt) {
    switch (evt->type) {
        case MQTT_EVT_CONNACK:
            printk("MQTT client connected!\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printk("MQTT client disconnected!\n");
            break;
        case MQTT_EVT_PUBLISH:
            printk("Message published.\n");
            break;
        default:
            printk("Unknown MQTT evt type: %u\n", evt->type);
            break;
    }
}

int mqtt_configure(struct mqtt_client *client) {
    struct sockaddr_in broker;
    
    broker.sin_family = AF_INET;
    broker.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &broker.sin_addr);

    struct mqtt_client_init_param init_params;
    memset(&init_params, 0, sizeof(init_params));

    init_params.broker = (struct sockaddr *)&broker;
    init_params.client_id.utf8 = (uint8_t*)"esp32_client";
    init_params.client_id.size = strlen("esp32_client");
    init_params.protocol_version = MQTT_VERSION_3_1_1;
    init_params.evt_cb = mqtt_event_handler;
    
    return mqtt_init(client, &init_params);
}

void mqtt_publish_state(struct mqtt_client *client) {
    struct mqtt_publish_param param;
    memset(&param, 0, sizeof(param));

    param.message.topic.qos = MQTT_QOS_1_AT_LEAST_ONCE;
    param.message.topic.topic.utf8 = (uint8_t *)MQTT_TOPIC;
    param.message.topic.topic.size = strlen(MQTT_TOPIC);
    param.message.payload.data = MSG_CONTENT;
    param.message.payload.len = strlen(MSG_CONTENT);
    
    mqtt_publish(client, &param);
}

void main(void) {
    int err;
    struct mqtt_client client;

    err = mqtt_configure(&client);
    if (err) {
        printk("Failed to configure MQTT client: %d\n", err);
        return;
    }

    err = mqtt_connect(&client);
    if (err) {
        printk("Failed to connect MQTT client: %d\n", err);
        return;
    }

    while (1) {
        mqtt_live(&client);

        mqtt_publish_state(&client);

        k_sleep(K_MSEC(PUBLISH_INTERVAL));
    }
}