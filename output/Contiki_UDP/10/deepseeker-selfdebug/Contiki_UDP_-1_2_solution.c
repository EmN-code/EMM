
#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/simple-udp.h"
#include "sys/log.h"

#define LOG_MODULE "Device State"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_CLIENT_PORT 12345
#define UDP_SERVER_PORT 20001
#define UDP_SERVER_IP "47.102.103.1"

static struct simple_udp_connection udp_conn;

PROCESS(device_state_process, "Device State Process");
AUTOSTART_PROCESSES(&device_state_process);

static void send_device_state(void) {
    static char state[] = "work"; // or "not work" based on device state
    uip_ipaddr_t server_ip;

    uiplib_ipaddrconv(UDP_SERVER_IP, &server_ip);
    simple_udp_sendto(&udp_conn, state, sizeof(state), &server_ip);
}

PROCESS_THREAD(device_state_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, NULL);

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        send_device_state();
        etimer_reset(&timer);
    }

    PROCESS_END();
}
