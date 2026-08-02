#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <nng/nng.h>

static volatile sig_atomic_t running = 1;
static void sigint_handler(int sig) {
    printf("recv sig [%d] and will exit now\n", sig);
    exit(0);
}

static void pipe_cb(nng_pipe pipe, nng_pipe_ev ev, void *arg) {
    (void)arg;
    uint64_t id = nng_pipe_id(pipe);
    if (ev == NNG_PIPE_EV_ADD_POST) {
        running = 1;
        printf("[pipe] %llu added\n", (unsigned long long)id);
    } else if (ev == NNG_PIPE_EV_REM_POST) {
        running = 0;
        printf("[pipe] %llu removed\n", (unsigned long long)id);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);
        return 1;
    }
    nng_init(NULL);
    const char *url = argv[1];
    signal(SIGINT, sigint_handler);

    nng_socket sock;
    int rv;
    if ((rv = nng_push0_open(&sock)) != 0) {
        fprintf(stderr, "nng_push0_open: %s\n", nng_strerror(rv));
        return 1;
    }
    nng_socket_set_ms(sock, NNG_OPT_SENDTIMEO, 100);
    nng_pipe_notify(sock, NNG_PIPE_EV_ADD_POST, pipe_cb, NULL);
    nng_pipe_notify(sock, NNG_PIPE_EV_REM_POST, pipe_cb, NULL);

    if ((rv = nng_dial(sock, url, NULL, 0)) != 0) {
        fprintf(stderr, "nng_dial: %s\n", nng_strerror(rv));
        nng_socket_close(sock);
        nng_fini();
        return 1;
    }
    printf("PUSH Client dialing to %s\n", url);

    int cnt = 0;
    while (true) {
        if (!running) {
            usleep(1000);
        }
        char msg[128];
        snprintf(msg, sizeof(msg), "Push data %d", cnt++);
        if ((rv = nng_send(sock, msg, strlen(msg)+1, 0)) != 0) {
            fprintf(stderr, "nng_send: %s\n", nng_strerror(rv));
            continue;
        }
        printf("[PUSH] %s\n", msg);
        sleep(1);
    }
    nng_socket_close(sock);
    nng_fini();
    return 0;
}
