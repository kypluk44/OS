#include <stdio.h>
#include <unistd.h> //row
#include <stdlib.h> //exit
#include <string.h>
#include <sys/types.h> //_t
#include <sys/wait.h> //wait

#define BUFSZ 256

int main(void) {
    int pipe1[2], pipe2[2];
    pid_t child_pid;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) { //дочерний процесс
        //проверка закрытости
        close(pipe1[1]);
        close(pipe2[0]);

        //переназначение стандартных потоков
        dup2(pipe1[0], STDIN_FILENO); //stdin ребёнка читается из pipe1
        dup2(pipe2[1], STDOUT_FILENO); //stdout ребёнка записывает в pipe2

        //закрытие
        close(pipe1[0]);
        close(pipe2[1]);

        //ребенок - копия родителя, поэтому выбираем выполнение child.c
        execl("./child", "./child", NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else { //родительский процесс
        //проверка закрытости
        close(pipe1[0]);
        close(pipe2[1]);
        
        char buffer[BUFSZ];
        char errbuf[BUFSZ];
        size_t r;

        printf("Введите имя файла: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Ошибка чтения\n");
            exit(EXIT_FAILURE);
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        write(pipe1[1], buffer, strlen(buffer));
        write(pipe1[1], "\n", 1);

        while (1) {
            printf("Введите строку (пустая строка/Ctrl+D для выхода): ");
            if (fgets(buffer, BUFSZ, stdin) == NULL) break; //ctrl+D == null
            buffer[strcspn(buffer, "\n")] = '\0';

            if (strlen(buffer) == 0) break; //enter == null

            write(pipe1[1], buffer, strlen(buffer));
            write(pipe1[1], "\n", 1);
        }

        close(pipe1[1]); //eof ребенку

        while ((r = read(pipe2[0], errbuf, sizeof(errbuf) - 1)) > 0) {
            errbuf[r] = '\0';
            printf("%s", errbuf);
        }

        close(pipe2[0]);
        wait(NULL);
    }

    return 0;
}
