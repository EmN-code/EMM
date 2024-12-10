#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "net/ip/uip.h"
#include "os/sys/etimer.h"
#include "os/sys/ctimer.h"
#include "os/sys/process.h"
#include "os/sys/pt.h"
#include "os/net/ipv6/simple-udp.h"
#include "os/sys/clock.h"

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer periodic_timer;
    static char msg[] = "work";
    uip_ipaddr_t dest_ip;
    
    PROCESS_BEGIN();

    // Set the remote IP address and port
    uiplib_ipaddrconv("47.102.103.1", &dest_ip);
    simple_udp_register(&udp_conn, 1234, &dest_ip, 20001, NULL);

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        // Send the device state.
        simple_udp_send(&udp_conn, msg, sizeof(msg));

        // Reset the timer to send the state every 5 seconds
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}