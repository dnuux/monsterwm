#define _POSIX_C_SOURCE 2

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define INTERVAL     5
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
print_cmus_status()
{
    char *status = NULL;
    char artist[64] = "";
    char title[128] = "";
    char buf[128];

    FILE *p;
    if (!(p = popen("cmus-remote -Q 2>/dev/null", "r")))
        return;

    while (fgets(buf, sizeof(buf), p) != NULL) {
        if (buf[0] == 's' && buf[1] == 't') {
            status = strchr(buf, ' ') + 1;
            if (status[0] != 'p' || status[1] != 'l') {
                pclose(p);
                printf(MUSIC_STR "Paused    ");
                return;
            }
        } else if (strncmp(buf, "tag ti", 6) == 0) {
            sscanf(buf, "%*s %*s %[^\n]", title);
            if (*artist) break;
        } else if (strncmp(buf, "tag ar", 6) == 0) {
            sscanf(buf, "%*s %*s %[^\n]", artist);
            if (*title) break;
        }
    }

    pclose(p);

    if (status) printf(MUSIC_STR "%s - %s    ", artist, title);
    else        printf(MUSIC_STR "Not running    ");
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

int main(void) {
    FILE *cpuinfo_fp;
    if (!(cpuinfo_fp = fopen(CPU_FILE, "r")))
        return 1;

    /* get initial jiffies */
    fscanf(cpuinfo_fp, "cpu %u %u %u %u %u %u %u",
            &jif1, &jif2, &jif3, &jif4, &jif5, &jif6, &jif7);
    fclose(cpuinfo_fp);

    /* the main loop */
    while (1) {
        print_cmus_status();
        print_updates();
        print_cpu_usage();
        print_memory_usage();
        print_date();
        fflush(stdout);
        sleep(INTERVAL);
    }

    return 0;
}

