
#include "contiki.h"
#include "net/mqtt.h"
#include "sys/etimer.h"
#include <stdio.h>

#define MQTT_BROKER_HOST "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

static struct mqtt_connection conn;
static struct etimer et;

PROCESS(mqtt_publish_process, "MQTT Publish Process");
AUTOSTART_PROCESSES(&mqtt_publish_process);

static void
mqtt_publish_state(void)
{
    mqtt_status_t status;
    uint16_t mid;

    status = mqtt_connect(&conn, MQTT_BROKER_HOST, MQTT_BROKER_PORT, 60, 1, NULL);
    if (status != MQTT_STATUS_OK) {
        printf("MQTT connect failed\n");
        return;
    }

    status = mqtt_publish(&conn, &mid, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), 0, 0, NULL);
    if (status != MQTT_STATUS_OK) {
        printf("MQTT publish failed\n");
    } else {
        printf("MQTT publish succeeded\n");
    }

    mqtt_disconnect(&conn);
}

PROCESS_THREAD(mqtt_publish_process, ev, data)
{
    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        mqtt_publish_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void)
{
    process_start(&mqtt_publish_process, NULL);
    return 0;
}
