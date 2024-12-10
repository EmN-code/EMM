#include <zephyr/net/socket_poll.h>
#include <zephyr/net/socket_select.h>

#include <zephyr.h>
#include <mqtt_client.h>
#include <net/socket.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883

/* State to be published: "work or not work" */
const char *states[] = {"work", "not work"};

void main(void) {
    struct mqtt_client client;
    uint8_t rx_buffer[128];
    uint8_t tx_buffer[128];
    struct sockaddr_in broker;
    int ret = 0;
    int state_index = 0;

    mqtt_client_init(&client);

    /* Set broker details */
    broker.sin_family = AF_INET;
    broker.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &broker.sin_addr);

    client.broker = &broker;
    client.client_id.utf8 = (uint8_t *)"esp32";
    client.client_id.size = strlen("esp32");

    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    ret = mqtt_connect(&client);

    if (ret != 0) {
        printk("Could not connect to MQTT broker, error: %d\n", ret);
        return;
    }

    printk("Connected to MQTT broker at %s:%d\n", SERVER_ADDR, SERVER_PORT);
    
    while (1) {
        /* Toggle state to publish */
        const char *state = states[state_index];
        struct mqtt_publish_param param;

        param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
        param.message.topic.topic.utf8 = (uint8_t *)"esp32/state";
        param.message.topic.topic.size = strlen("esp32/state");
        param.message.payload.data = (uint8_t *)state;
        param.message.payload.len = strlen(state);

        ret = mqtt_publish(&client, &param);

        if (ret != 0) {
            printk("Could not publish message: %d\n", ret);
        } else {
            printk("Published: %s\n", state);
        }

        /* Sleep for 5 seconds */
        k_sleep(K_SECONDS(5));

        /* Toggle between "work" and "not work" */
        state_index = (state_index + 1) % 2;
    }
}