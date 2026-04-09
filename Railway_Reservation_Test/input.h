#include <stdio.h>
#include <string.h>

int readInt(const char *msg, int min);
int readString(const char *msg, char *buf, int size);

int readInt(const char *msg, int min)
{
    int val;
    while (1)
    {
        printf("%s", msg);
        if (scanf("%d", &val) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid input! Please enter an integer.\n");
            continue;
        }
        while (getchar() != '\n');
        if (val < min)
        {
            printf("Invalid input! Value must be >= %d.\n", min);
            continue;
        }
        return val;
    }
}


int readString(const char *msg, char *buf, int size)
{
    int ch;
    printf("%s", msg);
    if (!fgets(buf, size, stdin))
        return 0;

    if (strchr(buf, '\n') == NULL)
    {
        printf("Error: input too long (max %d characters allowed)\n", size - 1);
        while ((ch = getchar()) != '\n' && ch != EOF);
        return 0;
    }

    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}
