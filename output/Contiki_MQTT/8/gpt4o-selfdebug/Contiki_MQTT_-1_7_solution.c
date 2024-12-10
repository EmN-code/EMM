#include <stdio.h>
#include <stdint.h>
#include <mqtt.h>
#include <net/mqtt.h>
#include <net/conn/udp.h>
#include <xtimer.h>

// Main function for setting up the MQTT connection and publishing repeatedly
int main(void) {
    struct mqtt_connection conn;
    uint16_t mid;
    char *mqtt_host = "47.102.103.1";
    uint16_t mqtt_port = 1883;
    char *topic = "state";
    uint8_t *message_work = (uint8_t *)"work";
    uint8_t *message_not_work = (uint8_t *)"not work";
    mqtt_qos_level_t qos_level = MQTT_QOS_0;

    mqtt_register(&conn, broker, mqtt_host, mqtt_con_cb, mqtt_disconnect_cb, mqtt_publish_cb);
    mqtt_connect(&conn, mqtt_host, mqtt_port, 60, 1, NULL);

    while (1) {
        mqtt_publish(&conn, &mid, topic, message_work, sizeof(message_work), qos_level);
        xtimer_sleep(5);
        // Optionally publish "not work" to simulate different states
        mqtt_publish(&conn, &mid, topic, message_not_work, sizeof(message_not_work), qos_level);
        xtimer_sleep(5);
    }

    // Clean disconnect
    mqtt_disconnect(&conn, NULL);
    return 0;
}

// Callback functions for MQTT (stub implementations)
static void mqtt_con_cb(struct mqtt_connection *conn, void *arg, mqtt_connection_status_t status) {}
static void mqtt_disconnect_cb(struct mqtt_connection *conn, void *arg, mqtt_connection_status_t status) {}
static void mqtt_publish_cb(struct mqtt_connection *conn, uint16_t pid, void *arg, mqtt_pub_status_t status) {}