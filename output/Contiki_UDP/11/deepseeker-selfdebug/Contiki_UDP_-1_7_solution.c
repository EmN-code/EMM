
#include "contiki.h"
#include "net/ipv4/uip.h"
#include "net/ipv4/uip-udp-packet.h"
#include "sys/etimer.h"
#include <string.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"

static struct uip_udp_conn *conn;
static struct etimer et;
static char device_state[10];

void udp_send_state(void) {
    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, 47, 102, 103, 1);

    if (strcmp(device_state, DEVICE_STATE_WORK) == 0) {
        strcpy(device_state, DEVICE_STATE_NOT_WORK);
    } else {
        strcpy(device_state, DEVICE_STATE_WORK);
    }

    uip_udp_packet_send(conn, device_state, strlen(device_state));
}

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    conn = uip_udp_new(&server_ip, UIP_HTONS(UDP_SERVER_PORT));
    if (conn == NULL) {
        PROCESS_EXIT();
    }

    strcpy(device_state, DEVICE_STATE_WORK);

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        udp_send_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&udp_client_process, NULL);
    return 0;
}
