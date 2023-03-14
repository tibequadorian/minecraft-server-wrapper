#include <signal.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/wait.h>

#define STOP_SIGNAL SIGTERM
#define STOP_COMMAND "stop\n"

int fd[2];

void sighandler(int signum) {
    write(fd[1], STOP_COMMAND, sizeof(STOP_COMMAND));
}

int main(int argc, char* argv[]) {
    pipe(fd);

    signal(STOP_SIGNAL, sighandler);

    // fork child process
    pid_t cid = fork();

    if (cid == 0) {
        // read from pipe output
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        // execute wrapped command
        execvp(argv[1], &argv[1]);

    } else {
        // write standard input to pipe
        close(fd[0]);
        char buffer[128];
        ssize_t nread;
        while ((nread = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
            write(fd[1], buffer, nread);
        }
        // wait for child
        wait(NULL);
    }
}
