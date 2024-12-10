
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "sys/etimer.h"
#include "sys/process.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct uip_udp_conn *conn;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void coap_response_handler(coap_message_t *response) {
    if(response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("Response received\n");
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;
    coap_message_t request;
    coap_message_t response;
    uint8_t buffer[128];

    PROCESS_BEGIN();

    conn = udp_new(NULL, UIP_HTONS(0), NULL);
    udp_bind(conn, UIP_HTONS(0));

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(&request, URI_PATH);
        coap_set_payload(&request, "Hello, COAP Cloud", 16);

        coap_send_request(&request, conn, SERVER_IP, SERVER_PORT, coap_response_handler);

        etimer_reset(&et);
    }

    PROCESS_END();
}
