#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <writefile> <writestr>\n", argv[0]);
        return 1;
    }

    openlog(NULL, 0, LOG_USER);

    FILE* file = fopen(argv[1], "w");
    if (!file) {
        syslog(LOG_ERR, "Failed to open file!\n");
        return 1;
    }
    if (fprintf(file, "%s\n", argv[2]) < 0) {
        syslog(LOG_ERR, "Failed to write string!\n");
    }
    if (fclose(file) != 0) {
        syslog(LOG_ERR, "Failed to close file!\n");
    }

    syslog(LOG_DEBUG, "Writing %s to %s\n", argv[2], argv[1]);
    
    return 0;
}