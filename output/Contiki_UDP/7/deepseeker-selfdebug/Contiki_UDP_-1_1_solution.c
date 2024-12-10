
#include "contiki.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-debug.h"
#include "net/ipv6/simple-udp.h"

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define DEVICE_STATE_WORK "work"
#define DEVICE_STATE_NOT_WORK "not work"

static struct simple_udp_connection udp_conn;
static char device_state[10];

PROCESS(device_state_sender, "Device State Sender");
AUTOSTART_PROCESSES(&device_state_sender);

static void udp_callback(struct simple_udp_connection *c,
                         const uip_ipaddr_t *sender_addr,
                         uint16_t sender_port,
                         const uip_ipaddr_t *receiver_addr,
                         uint16_t receiver_port,
                         const uint8_t *data,
                         uint16_t datalen)
{
    // Optional: Handle incoming UDP messages if needed
}

PROCESS_THREAD(device_state_sender, ev, data)
{
    static struct etimer et;
    static int work_state = 1; // 1 for work, 0 for not work

    PROCESS_BEGIN();

    // Initialize UDP connection
    simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, udp_callback);

    // Set the initial device state
    strcpy(device_state, DEVICE_STATE_WORK);

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&et, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Toggle the device state
        if (work_state) {
            strcpy(device_state, DEVICE_STATE_NOT_WORK);
            work_state = 0;
        } else {
            strcpy(device_state, DEVICE_STATE_WORK);
            work_state = 1;
        }

        // Send the device state to the UDP server
        uip_ipaddr_t server_ip;
        uip_ipaddr(&server_ip, UDP_SERVER_IP);
        simple_udp_sendto(&udp_conn, device_state, strlen(device_state), &server_ip);
    }

    PROCESS_END();
}
