
#include <stdio.h>
#include <string.h>
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-debug.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/etimer.h"
#include "sys/process.h"
#include "sys/log.h"

#define LOG_MODULE "CoAP Client"
#define LOG_LEVEL LOG_LEVEL_INFO

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static struct coap_resource_t coap_resource;
static struct coap_endpoint_t coap_server;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void coap_response_handler(coap_message_t *response) {
    if(response == NULL) {
        LOG_INFO("No response received\n");
        return;
    }

    char *payload = (char *)coap_get_payload(response);
    if(payload) {
        LOG_INFO("Response received: %s\n", payload);
    } else {
        LOG_INFO("Empty response received\n");
    }
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;
    static coap_message_t request;

    PROCESS_BEGIN();

    LOG_INFO("Starting CoAP client\n");

    coap_endpoint_parse(COAP_SERVER_IP, strlen(COAP_SERVER_IP), &coap_server);
    coap_server.port = COAP_SERVER_PORT;

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(&request, COAP_URI_PATH);
        coap_set_payload(&request, "Hello, COAP Cloud", 16);

        if(coap_send_request(&request, &coap_server, coap_response_handler) == COAP_SEND_SUCCESS) {
            LOG_INFO("CoAP request sent\n");
        } else {
            LOG_INFO("Failed to send CoAP request\n");
        }

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    PROCESS_PAUSE();
    return 0;
}
