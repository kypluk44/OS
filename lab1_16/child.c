#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFSIZE 256

int main(void) {
    char buffer[BUFSIZE];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        const char *msg = "Не удалось прочитать имя файла\n";
        if (write(STDERR_FILENO, msg, strlen(msg)) != (ssize_t)strlen(msg)){
            perror("write");
            exit(EXIT_FAILURE);
        };
        return 1;
    }
    buffer[strcspn(buffer, "\n")] = '\0';

    FILE *fp = fopen(buffer, "w");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';

        ssize_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '.' || buffer[len - 1] == ';')) {
            fprintf(fp, "%s\n", buffer);
            fflush(fp);
        } else {
            char err[BUFSIZE];
            sprintf(err, "Error: строка должна заканчиваться на '.' или ';' - '%s'\n", buffer);
            if (write(STDOUT_FILENO, err, strlen(err)) != (ssize_t)strlen(err)){
                perror("write");
                exit(EXIT_FAILURE);
            };
        }
    }

    fclose(fp);
    return 0;
}
