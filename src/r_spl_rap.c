#include <r_spl_rap.h>

#include <string.h>

#include <r_socket.h>
#include <r_spl_core.h>
#include <r_util/r_str.h>

RSocket * r_spl_rap_open(const char *host, const char *port, const char *file) {
    RSocket *socket = r_socket_new(false);

    CALL_AND_REPORT(NULL, r_socket_connect, socket, host, port, R_SOCKET_PROTO_TCP, 1000);
    CALL_AND_REPORT(NULL, r_socket_rap_client_open, socket, file, 1);
    // Required otherwise the "cmd_len" is wrong (or something idk)
    R_SPL_RAP_COMMAND(e scr.color=1);

    if (strncmp(file, "dbg://", 6) == 0) {
        char * const r = r_socket_rap_client_command(socket, "ood", NULL);
        bool ret = false;
        if (r != NULL) {
            r_str_trim(r);
            if (strlen(r) == 0) {
                printf("ERR: failed to start debug session\n");
                ret = true;
            } else
                printf("%s\n", r);

            free(r);
        }
        if (ret)
            exit(1);
    }

    return socket;
}

void r_spl_rap_command(RSocket *socket, const char *cmd) {
    char * const r = r_socket_rap_client_command(socket, cmd, NULL);
    if (r != NULL) {
        r_str_trim(r);
        if (strlen(r) != 0)
            printf("%s\n", r);
        free(r);
    }
}
