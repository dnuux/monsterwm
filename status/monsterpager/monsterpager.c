#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define DESKTOPS     4
#define DEFAULT      "#909090"
#define DEFAULTBG    "#151515"
#define CURRENT      "#EEEEEE"
#define CURRENTBG    "#31658C"
#define OCCUPIED     "#EEEEEE"
#define OCCUPIEDBG   "#333333"
#define LAYOUT       "#909090"
#define ICONSPATH    "/home/elias/pictures/status/"
#define ICON(x)      "  ^i("ICONSPATH x") "

static const char *desktop[DESKTOPS] = {
    " " ICON("screen.xbm")      "term",
        ICON("fox.xbm")         "web",
        ICON("dev.xbm")         "dev",
        ICON("net_down_01.xbm") "torrent",
};

static const char layouts[] = { 'T', 'M', 'B', 'G', 'F' };

int main(int argc, const char *argv[])
{
    FILE *f;
    int fd;
    char *next, buffer[32];
    unsigned i, w, m, c, mode = 0;

    if (argc < 2) return 1;

    if ((f = fopen(argv[1], "r")))
        fclose(f);
    else if (mkfifo(argv[1], 0600) == -1)
        return 1;

    if ((fd = open(argv[1], O_RDONLY)) == 1)
        return 1;

    while (read(fd, buffer, sizeof(buffer))) {
        if (buffer[1] != ':' || buffer[3] != ':') {
            memset(buffer, '\0', sizeof(buffer));
            continue;
        }

        next = buffer;
        for (i = 0; i < DESKTOPS; ++i) {
            sscanf(next, "%u:%u:%u", &w, &m, &c);
            if (c) {    mode = m;
                        printf("^bg(%s)^fg(%s)%s  ", CURRENTBG, CURRENT, desktop[i]); }
            else if (w) printf("^bg(%s)^fg(%s)%s  ", OCCUPIEDBG, OCCUPIED, desktop[i]);
            else        printf("^bg(%s)^fg(%s)%s  ", DEFAULTBG, DEFAULT, desktop[i]);

            if (i != DESKTOPS-1) next = strchr(next + 5, ' ') + 1;
        }

        printf("^bg()  ^fg(" LAYOUT ")[%c]\n", layouts[mode]);
        fflush(stdout);
    }

    close(fd);
    unlink(argv[1]);

    return 0;
}

