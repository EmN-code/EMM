#include "time_units.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "esp_wifi.h"
#include "arpa/inet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("Setting up WiFi...\n");
    // Assuming WiFi configuration is handled and the ESP32 is connected to a network.
    
    // Define server and CoAP message details
    const char *server_ip = "47.102.103.1";
    uint16_t server_port = 5683;
    const char *uri_path = "/coap/test";
    char message[] = "Hello, COAP Cloud";
    
    // Setup the server endpoint
    sock_udp_ep_t server;
    memset(&server, 0, sizeof(server));
    server.family = AF_INET;
    server.netif = SOCK_ADDR_ANY_NETIF;
    server.port = server_port;
    inet_pton(AF_INET, server_ip, &server.addr.ipv4);

    // Create CoAP PDU
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t bytes;
    
    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, uri_path);
    // Set payload
    bytes = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, message, sizeof(message));
    bytes += sizeof(message);

    // Send the request
    ssize_t result = gcoap_req_send(buf, bytes, &server, NULL);

    if (result <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP POST request sent\n");
    
    // Usually, we wait for a response here, but this depends on the specific use case
    
    return 0;
}