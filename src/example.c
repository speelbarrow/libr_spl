#include <signal.h>

#include <r_socket.h>
#include <r_spl.h>

int main(int argc, char *argv[], char *envp[]) {
    R_SPL_SETUP();

    pid_t pid = fork();
    switch (pid) {
        case -1:
            write(2, "fork failed\n", 12);
            break;
        case 0:

            execve("/usr/local/bin/radare2", (char * const []){"/usr/local/bin/radare2", "rap://:9090/", NULL}, envp);
            // If execve fails, kill the process
            printf("child failed\n");
            return 1;
    }

    sleep(1);

    RSocket *socket = r_spl_rap_open("localhost", "9090", "/bin/ls");

    R_SPL_RAP_COMMAND(s main);
    R_SPL_RAP_COMMAND(pd);

    kill(pid, SIGKILL);

    R_SPL_TEARDOWN();
}
