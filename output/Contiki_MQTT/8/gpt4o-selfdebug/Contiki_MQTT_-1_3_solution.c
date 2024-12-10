#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcp-socket.h"
#include "mqtt.h"

#define MAX_TCP_SEGMENT_SIZE 1024  // Defined MAX_TCP_SEGMENT_SIZE for TCP communication

PROCESS(mqtt_publisher_process, "MQTT Publisher");
AUTOSTART_PROCESSES(&mqtt_publisher_process);

static struct mqtt_connection conn;
static char client_id[32];
static char *pub_topic = "state/update";
static struct etimer pub_timer;

PROCESS_THREAD(mqtt_publisher_process, ev, data) {
    PROCESS_BEGIN();

    // Configure the client ID
    snprintf(client_id, sizeof(client_id), "nrf52840_mqtt");

    // Establishing a connection
    mqtt_status_t status = mqtt_register(&conn, &mqtt_publisher_process, client_id, NULL, MAX_TCP_SEGMENT_SIZE);
    if (status != MQTT_STATUS_OK) {
        printf("Failed to register MQTT connection\n");
        PROCESS_EXIT();
    }

    // Connect to MQTT broker
    status = mqtt_connect(&conn, "47.102.103.1", 1883, 60, 1, NULL);  // Corrected to include 'clean_session' and 'prop_list'
    if (status != MQTT_STATUS_OK) {
        printf("Failed to connect to MQTT broker\n");
        PROCESS_EXIT();
    }

    while (1) {
        etimer_set(&pub_timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&pub_timer));

        status = mqtt_publish(&conn, NULL, pub_topic, (uint8_t *)"work", strlen("work"), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF, 0, MQTT_TOPIC_ALIAS_DISABLED, NULL);
        if (status == MQTT_STATUS_OK) {
            printf("Message published: work\n");
        } else {
            printf("Failed to publish message\n");
        }
    }

    PROCESS_END();
}
