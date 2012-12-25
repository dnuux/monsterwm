#define _POSIX_C_SOURCE 2

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <mpd/client.h>

#define INTERVAL     5
#define MPD_PORT     6600
#define MPD_TIMEOUT  1000
#define CPU_FILE     "/proc/stat"
#define MEM_FILE     "/proc/meminfo"
#define UPDATE_FILE  "/home/elias/.pacmanupdates"
#define TIME_FORMAT  "%d/%m/%Y %H:%M"

#define ICON(h,i)    "^fg("h")^i(/home/elias/pictures/status/"i")^fg()  "
#define MUSIC_STR    ICON("#cc7683", "note.xbm")
#define UPDT_STR     ICON("#cc7836", "pacman.xbm")
#define CPU_STR      ICON("#b1d631", "cpu.xbm")
#define MEM_STR      ICON("#31658C", "memory.xbm")
#define DATE_STR     ICON("#cc7836", "clock.xbm")

static unsigned jif1, jif2, jif3, jif4, jif5, jif6, jif7;
static unsigned times = 1;
static char pacbuf[16];

static void
print_mpd_status()
{
    struct mpd_connection *conn = mpd_connection_new("localhost", MPD_PORT, MPD_TIMEOUT);
    if (mpd_connection_get_error(conn)) {
        mpd_connection_free(conn);
        printf(MUSIC_STR "Not running    ");
        return;
    }

    struct mpd_status *status = mpd_run_status(conn);
    if (status) {
        const enum mpd_state state = mpd_status_get_state(status);
        mpd_status_free(status);

        if (state == MPD_STATE_PAUSE || state == MPD_STATE_STOP) {
            printf(MUSIC_STR "Paused    ");
            mpd_connection_free(conn);
            return;
        }
    }

    struct mpd_song *song = mpd_run_current_song(conn);
    if (song) {
        const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
        const char *title  = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);

        printf(MUSIC_STR "%s - %s    ", artist, title);
        mpd_song_free(song);
    }

    mpd_connection_free(conn);
}

static void
print_updates()
{
    if (--times == 0) {
        char buf[12];
        unsigned core = 0, extra = 0, community = 0, repo_ck = 0;

        FILE *updt_fp;
        if (!(updt_fp = fopen(UPDATE_FILE, "r")))
            return;

        while (fgets(buf, sizeof(buf), updt_fp) != NULL) {
            if (buf[1] == 0) break;
            switch (buf[2]) {
                case 'r':      ++core; break;
                case 't':     ++extra; break;
                case 'm': ++community; break;
                case 'p':   ++repo_ck; break;
            }
        }

        fclose(updt_fp);
        snprintf(pacbuf, sizeof(pacbuf), "%u / %u / %u / %u",
                 core, extra, community, repo_ck);
        times = 12;
    }

    printf(UPDT_STR "%s    ", pacbuf);
}

static void
print_cpu_usage()
{
    FILE *cpuinfo_fp;
    unsigned lnum1, lnum2, lnum3, lnum4, lnum5, lnum6, lnum7;

    if (!(cpuinfo_fp = fopen(CPU_FILE, "r")))
        return;

    fscanf(cpuinfo_fp, "cpu %u %u %u %u %u %u %u",
            &lnum1, &lnum2, &lnum3, &lnum4, &lnum5, &lnum6, &lnum7);
    fclose(cpuinfo_fp);

    printf(CPU_STR "%u%%    ", (unsigned) (100 * ((float)
          (lnum1 - jif1 + lnum2 - jif2 + lnum3 - jif3) /
          (lnum1 - jif1 + lnum2 - jif2 + lnum3 - jif3 +
           lnum4 - jif4 + lnum5 - jif5 + lnum6 - jif6 +
           lnum7 - jif7))));

    jif1 = lnum1; jif2 = lnum2; jif3 = lnum3; jif4 = lnum4;
    jif5 = lnum5; jif6 = lnum6; jif7 = lnum7;
}

static void
print_memory_usage()
{
    FILE *meminfo_fp;
    unsigned memmax, memfree, buffers, cached;

    if (!(meminfo_fp = fopen(MEM_FILE, "r")))
        return;

    fscanf(meminfo_fp, "MemTotal: %u kB\nMemFree: %u kB\nBuffers: %u kB\nCached: %u",
            &memmax, &memfree, &buffers, &cached);
    fclose(meminfo_fp);

    printf(MEM_STR "%uMiB    ", (memmax - memfree - buffers - cached) / 1024);
}

static void
print_date()
{
    char buf[20];
    time_t rawtime = time(NULL);
    strftime(buf, sizeof(buf) - 1, TIME_FORMAT, localtime(&rawtime));

    printf(DATE_STR "%s \n", buf);
}

int
main(void)
{
    FILE *cpuinfo_fp;
    if (!(cpuinfo_fp = fopen(CPU_FILE, "r")))
        return 1;

    /* get initial jiffies */
    fscanf(cpuinfo_fp, "cpu %u %u %u %u %u %u %u",
            &jif1, &jif2, &jif3, &jif4, &jif5, &jif6, &jif7);
    fclose(cpuinfo_fp);

    /* the main loop */
    while (1) {
        print_mpd_status();
        print_updates();
        print_cpu_usage();
        print_memory_usage();
        print_date();
        fflush(stdout);
        sleep(INTERVAL);
    }

    return 0;
}

