#include <coap-request-state.h>  // CoAP state handling
#include <contiki-default-conf.h>  // Default configuration for the Contiki OS
#include <contiki-conf.h> // Contiki OS configuration

#include <stdio.h>
#include <string.h>

// Define the endpoint for CoAP server
#define SERVER_EP "coap://47.102.103.1:5683"
// Define the message
#define POST_MSG "Hello, COAP Cloud"
// Define the URI path
#define URI_PATH "/coap/test"

PROCESS(coap_post_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_post_client_process);

PROCESS_THREAD(coap_post_client_process, ev, data)
{
    static struct etimer timer;
    static coap_packet_t request[1]; // Handling single packet request

    PROCESS_BEGIN();

    // Initialize the timer to send POST request every 10 seconds
    etimer_set(&timer, CLOCK_SECOND * 10);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Prepare the CoAP request
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, URI_PATH);
        coap_set_payload(request, POST_MSG, strlen(POST_MSG));

        printf("Sending CoAP POST Request to %s\n", SERVER_EP);

        // Send CoAP request
        COAP_BLOCKING_REQUEST(SERVER_EP, request, NULL);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main()
{
    process_start(&coap_post_client_process, NULL);
    // Run the Contiki processes - This may vary based on your environment configuration
    while(1) {
        // Simulation run loop goes here
        etimer_request_poll();
    }
    return 0;
}