#include "gnrc_netif.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

// Function to initialize the network interface
void init_network(void) {
    gnrc_netif_t *netif = gnrc_netif_get_by_pid(IPV4_PID); // Replace IPV4_PID with the actual PID of your IPv4 interface
    if (netif == NULL) {
        puts("Failed to get network interface");
        return;
    }

    // Ensure the network interface is up
    if (!gnrc_netif_is_up(netif)) {
        gnrc_netif_up(netif);
    }
}
#include "net/sock/udp.h"

// Function to create a UDP socket
sock_udp_t *create_udp_socket(void) {
    sock_udp_t *sock = sock_udp_create(NULL, NULL, NULL, 0);
    if (sock == NULL) {
        puts("Failed to create UDP socket");
        return NULL;
    }
    return sock;
}
#include "net/gcoap.h"

// Function to send a CoAP POST request
void send_coap_post_request(sock_udp_t *sock) {
    char payload[] = "Hello, COAP Cloud";
    char buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = 5683,
        .addr.ipv4 = INET_ADDR_STR("47.102.103.1")
    };

    // Initialize the CoAP packet
    int res = gcoap_req_init(&pdu, (uint8_t *)buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    if (res < 0) {
        puts("Failed to initialize CoAP request");
        return;
    }

    // Add the payload to the CoAP packet
    res = coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    if (res < 0) {
        puts("Failed to add format option");
        return;
    }

    res = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (res < 0) {
        puts("Failed to finish CoAP options");
        return;
    }

    res = coap_pkt_add_payload(&pdu, payload, strlen(payload));
    if (res < 0) {
        puts("Failed to add payload");
        return;
    }

    // Send the CoAP packet
    res = gcoap_req_send((uint8_t *)buf, coap_pkt_len(&pdu), &remote, NULL, NULL, SOCK_UDP);
    if (res < 0) {
        puts("Failed to send CoAP request");
        return;
    }

    puts("CoAP POST request sent successfully");
}
int main(void) {
    init_network();
    sock_udp_t *sock = create_udp_socket();
    if (sock != NULL) {
        send_coap_post_request(sock);
        sock_udp_close(sock);
    }
    return 0;
}