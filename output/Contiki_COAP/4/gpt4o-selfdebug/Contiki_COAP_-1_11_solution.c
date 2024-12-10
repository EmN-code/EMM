#include "contiki-net.h"
#include "coap-transactions.h"
#include "sys/timer.h"
#include "contiki-default-conf.h"

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer timer;

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize the etimer
    etimer_set(&timer, CLOCK_SECOND);

    // Define the server endpoint
    coap_endpoint_t server_endpoint = {
        .ipaddr = {47, 102, 103, 1},  // IP address
        .port = 5683  // Port
    };

    // Prepare the CoAP request
    static coap_packet_t request[1];  // Buffer for the request
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");

    while (1) {
        if (etimer_expired(&timer)) {
            coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 18);
            COAP_BLOCKING_REQUEST(&server_endpoint, request,
                                  NULL);  // NULL handler for simple requests
            // Reset the timer
            etimer_reset(&timer);
        }

        PROCESS_WAIT_EVENT();
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize Contiki
    process_init();
    // Start etimer process
    etimer_process();
    // Start all auto-start processes
    autostart_start(autostart_processes);

    while (1) {
        int r = process_run();
        if (r > 0) {
            // Call idle loop when no process is called
            etimer_request_poll();
        }
    }
    return 0;
}