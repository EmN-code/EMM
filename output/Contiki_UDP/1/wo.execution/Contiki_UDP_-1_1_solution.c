#include \<uip-ds6.h>\n#include \<uip.h>\n#include \<process.h>\n#include \<etimer.h>\n#include \<simple-udp.h>\n#include \<stdio.h>\n\n#define UDP_PORT 20001\n#define SERVER_IP "47.102.103.1"\n\nstatic struct simple_udp_connection udp_conn;\n\nPROCESS(udp_client_process, "UDP client process");\nAUTOSTART_PROCESSES(&udp_client_process);\n\nPROCESS_THREAD(udp_client_process, ev, data)\n{\n  static struct etimer timer;\n  static char message[20];\n  uip_ipaddr_t server_ipaddr;\n\n  PROCESS_BEGIN();\n\n  // Initialize the UDP connection\n  simple_udp_register(&udp_conn, UDP_PORT, NULL, UDP_PORT, NULL);\n\n  // Convert the string representation of the IP address to a uip_ipaddr_t\n  uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);\n\n  while(1) {\n    // Set a timer for 5 seconds\n    etimer_set(&timer, CLOCK_SECOND * 5);\n    \n    // Wait for the timer event\n    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));\n    \n    // Prepare the message\n    snprintf(message, sizeof(message), "work");\n\n    // Send the message to the server\n    simple_udp_sendto(&udp_conn, message, strlen(message) + 1, &server_ipaddr);\n    \n    printf("Sent message: %s\n", message);\n  }\n\n  PROCESS_END();\n}