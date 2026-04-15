#include <stdio.h>
#include <time.h>

int main() {
    time_t now;
    time(&now);
    char timeBuffer[64];

    strftime(timeBuffer, sizeof(timeBuffer),
             "Last Updated: %d-%m-%Y %H:%M:%S\n",
             localtime(&now));    

    printf("%s\n", timeBuffer);
    return 0;
}
