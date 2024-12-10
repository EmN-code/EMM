
#include <stdio.h>
#include "contiki.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "net/ip/uip.h"
#include "net/ip/uip-udp-packet.h"
#include "dev/button-hal.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_INTERVAL (CLOCK_SECOND * 5)

static struct uip_udp_conn *conn;
static char device_state[] = "work";

PROCESS(device_state_process, "Device State Process");
AUTOSTART_PROCESSES(&device_state_process);

void send_device_state(void) {
    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, 47, 102, 103, 1);
    uip_udp_packet_sendto(conn, device_state, sizeof(device_state) - 1, &server_ip, UIP_HTONS(UDP_SERVER_PORT));
}

PROCESS_THREAD(device_state_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    conn = uip_udp_new(&server_ip, UIP_HTONS(UDP_SERVER_PORT));
    if(conn == NULL) {
        printf("Failed to create UDP connection\n");
        PROCESS_EXIT();
    }

    etimer_set(&et, DEVICE_STATE_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        send_device_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    printf("Starting Device State Process\n");
    process_start(&device_state_process, NULL);
    return 0;
}
