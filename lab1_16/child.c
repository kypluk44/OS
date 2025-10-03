#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFSZ 256

int main(void) {
    char buffer[BUFSZ];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        const char *msg = "Не удалось прочитать имя файла\n";
        write(STDERR_FILENO, msg, strlen(msg));
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

        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '.' || buffer[len - 1] == ';')) {
            fprintf(fp, "%s\n", buffer);
            fflush(fp);
        } else {
            char err[BUFSZ];
            sprintf(err, "Error: строка должна заканчиваться на '.' или ';' - '%s'\n", buffer);
            write(STDOUT_FILENO, err, strlen(err));
        }
    }

    fclose(fp);
    return 0;
}
