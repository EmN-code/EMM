#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct mqtt_connection conn;
static struct process *app_process;
static uint16_t mid = 0;
static char topic[] = "device/state";
static char payload[] = "work";

PROCESS(mqtt_client_process, "MQTT Client");
AUTOSTART_PROCESSES(&mqtt_client_process);

static void mqtt_event_callback(mqtt_event_t event, void *ptr)
{
    switch(event) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_PUBLISHED:
            printf("Message published\n");
            break;
        default:
            printf("MQTT event: %d\n", event);
            break;
    }
}

int main()
{
    app_process = &mqtt_client_process;

    mqtt_register(&conn, app_process, "client_id", mqtt_event_callback, 128);

    mqtt_connect(&conn, "47.102.103.1", 1883, 60, MQTT_CLEAN_SESSION_ON, NULL);

    while(1) {
        mqtt_publish(&conn, &mid, topic, (uint8_t *)payload, strlen(payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etimer));
        etimer_set(&etimer, CLOCK_SECOND * 5);
    }

    return 0;
}

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    PROCESS_BEGIN();

    while(1) {
        PROCESS_WAIT_EVENT();
    }

    PROCESS_END();
}