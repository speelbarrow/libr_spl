#ifndef R_SPL_RAP_H
#define R_SPL_RAP_H

#include <r_socket.h>

/** 
 * Open a connection to a RAP server.
 *
 * radare2 functions:
 * `r_socket_new`, `r_socket_connect`, `r_socket_rap_client_open`, `r_socket_rap_client_command`
 */
RSocket * r_spl_rap_open(const char *host, const char *port, const char *file);

/**
 * Issue a command to a connected RAP `RSocket`.
 *
 * radare2 functions:
 * `r_socket_rap_client_command`
 */
void r_spl_rap_command(RSocket *socket, const char *cmd);

#define R_SPL_RAP_COMMAND(...) r_spl_rap_command(socket, #__VA_ARGS__)

#endif // R_SPL_RAP_H
