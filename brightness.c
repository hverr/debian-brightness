#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BACKLIGHT_DIR "/sys/class/backlight/acpi_video0"
#define MAX_BRIGHTNESS BACKLIGHT_DIR"/max_brightness"
#define ACTUAL_BRIGHTNESS BACKLIGHT_DIR"/actual_brightness"
#define CHANGE_BRIGHTNESS BACKLIGHT_DIR"/brightness"

void printUsage(FILE *fh, const char *pname);
int usage(void);
int readint(const char *filepath);
int current(void);
int maximum(void);
void brightnessShow(void);
void brightnessUp(void);
void brightnessDown(void);
void setbrightness(int level);

int main(int argc, const char **argv) {
    const char *cmd;

    if(argc != 2) {
        printUsage(stderr, argv[0]);
        return 1;
    }

    cmd = argv[1];
    if(strcmp(cmd, "up") == 0) {
        brightnessUp();

    } else if(strcmp(cmd, "down") == 0) {
        brightnessDown();

    } else if(strcmp(cmd, "show") == 0) {
        brightnessShow();

    } else {
        fprintf(stderr, "error: unknown command: %s\n", cmd);
        printUsage(stderr, argv[0]);
        return 1;
    }

    return 0;
}

void printUsage(FILE *fh, const char *pname) {
    fprintf(fh, "Usage: %s up\n", pname);
    fprintf(fh, "       %s down\n", pname);
    fprintf(fh, "       %s show\n", pname);
}

int readint(const char *path) {
    FILE *fh;
    int level, status;

    fh = fopen(path, "r");
    if(fh == NULL) {
        fprintf(stderr, "error: couldn't upen file %s: %s\n",
                path, strerror(errno));
        exit(1);
    }

    status = fscanf(fh, "%d", &level);
    fclose(fh);
    if(status != 1) {
        fprintf(stderr, "error: %s: couldn't read integer\n", path);
        exit(1);
    }

    return level;
}

int current(void) {
    return readint(ACTUAL_BRIGHTNESS);
}

int maximum(void) {
    return readint(MAX_BRIGHTNESS);
}

void brightnessShow(void) {
    int c, m, pct;

    c = current();
    m = maximum();
    pct = c*100/m;
    printf("Brightness %d: %d%%\n", c, pct);
}

void brightnessUp(void) {
    int c, m;

    c = current();
    m = maximum();
    
    if(c < m) {
        c++;
        setbrightness(c);
    }
}

void brightnessDown(void) {
    int c;

    c = current();
    if(c > 0) {
        c--;
        setbrightness(c);
    }
}

void setbrightness(int level) {
    const char *path = CHANGE_BRIGHTNESS;
    FILE *fh;
    int status;

    fh = fopen(path, "w");
    if(fh == NULL) {
        fprintf(stderr, "error: opening %s for writing: %s\n",
                path, strerror(errno));
        exit(1);
    }
    status = fprintf(fh, "%d\n", level);
    fclose(fh);
    if(status <= 0) {
        fprintf(stderr, "error: writing to %s: %s\n", path, strerror(errno));
        exit(1);
    }
}

