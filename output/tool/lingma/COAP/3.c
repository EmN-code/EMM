#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv4/addr.h>

// Define the CoAP server's IPv4 address and port
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

// Function to send a CoAP POST request
static void send_coap_post_request(void) {
    // Create a UDP socket
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_t sock;
    int res = sock_udp_create(&sock, &local, NULL, 0);
    if (res < 0) {
        printf("Failed to create UDP socket: %d\n", res);
        return;
    }

    // Prepare the CoAP request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (res < 0) {
        printf("Failed to initialize CoAP request: %d\n", res);
        sock_udp_close(&sock);
        return;
    }

    // Add the payload to the CoAP request
    const char *payload = "Hello, COAP Cloud";
    res = coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    if (res < 0) {
        printf("Failed to add format option: %d\n", res);
        sock_udp_close(&sock);
        return;
    }
    res = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (res < 0) {
        printf("Failed to finish CoAP options: %d\n", res);
        sock_udp_close(&sock);
        return;
    }
    res = coap_pkt_add_payload(&pdu, (uint8_t *)payload, strlen(payload));
    if (res < 0) {
        printf("Failed to add payload: %d\n", res);
        sock_udp_close(&sock);
        return;
    }

    // Prepare the destination endpoint
    sock_udp_ep_t remote;
    ipv4_addr_t server_ip;
    ipv4_addr_from_str(&server_ip, COAP_SERVER_IP);
    remote.addr.ipv4 = server_ip;
    remote.port = COAP_SERVER_PORT;

    // Send the CoAP request
    res = gcoap_req_send(buf, coap_pkt_len(&pdu), &remote, NULL, NULL, SOCK_UDP);
    if (res < 0) {
        printf("Failed to send CoAP request: %d\n", res);
    } else {
        printf("CoAP POST request sent successfully\n");
    }

    // Close the UDP socket
    sock_udp_close(&sock);
}

int main(void) {
    // Initialize the network stack
    gnrc_netif_init();

    // Send the CoAP POST request
    send_coap_post_request();

    return 0;
}