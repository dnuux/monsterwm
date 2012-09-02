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
    int fildes;
    FILE *fname;
    char *next, buffer[32];
    unsigned i, w, m, c, mode = 0;

    if (argc < 2) return 1;

    /* create the FIFO if it doesn't already exist */
    if ((fname = fopen(argv[1], "r")))
        fclose(fname);
    else if (mkfifo(argv[1], 0600) == -1)
        return 1;

    if ((fildes = open(argv[1], O_RDONLY)) == 1)
        return 1;

    /* print information as long as data is available */
    while (read(fildes, buffer, sizeof(buffer))) {

        /* check if right kind of data */
        if (buffer[1] != ':' || buffer[3] != ':') {
            memset(buffer, 0, sizeof(buffer));
            continue;
        }

        next = buffer;
        for (i = 0; i < DESKTOPS; ++i) {
            sscanf(next, "%u:%u:%u", &w, &m, &c);

            /* order of priority: is current, has windows, everyone else */
            if (c) {    mode = m;
                        printf("^bg(%s)^fg(%s)%s  ", CURRENTBG, CURRENT, desktop[i]); }
            else if (w) printf("^bg(%s)^fg(%s)%s  ", OCCUPIEDBG, OCCUPIED, desktop[i]);
            else        printf("^bg(%s)^fg(%s)%s  ", DEFAULTBG, DEFAULT, desktop[i]);

            /* find start position in buffer for next desktop */
            if (i != DESKTOPS-1) next = strchr(next + 5, ' ') + 1;
        }

        printf("^bg()  ^fg(" LAYOUT ")[%c]\n", layouts[mode]);
        fflush(stdout);
    }

    close(fildes);
    unlink(argv[1]);

    return 0;
}

