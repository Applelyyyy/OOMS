// github_sync.c by-ChatGPT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "github_sync.h"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#define DOWNLOAD_CMD(url, output) \
    "powershell -Command \"Invoke-WebRequest -Uri '" url "' -OutFile '" output "'\""
#else
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0777)
#define DOWNLOAD_CMD(url, output) \
    "wget -q -O \"" output "\" \"" url "\""
#endif

//if ต้องการที่จะเปลี่ยน raw_data.csv
#define LOCAL_PATH "../data/raw_data.csv"
#define TEMP_PATH  "../data/temp.csv"
#define REMOTE_URL "https://raw.githubusercontent.com/Applelyyyy/OOMS/main/data/raw_data.csv"

int file_exists(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) return 0;
    fclose(fp);
    return 1;
}

int compare_files(const char *path1, const char *path2) {
    FILE *f1 = fopen(path1, "rb");
    FILE *f2 = fopen(path2, "rb");
    if (!f1 || !f2) return 1;

    int diff = 0;
    while (!feof(f1) && !feof(f2)) {
        if (fgetc(f1) != fgetc(f2)) {
            diff = 1;
            break;
        }
    }
    if (!feof(f1) || !feof(f2)) diff = 1;

    fclose(f1);
    fclose(f2);
    return diff;
}

int download_file(const char *url, const char *output) {
    char cmd[512];
#ifdef _WIN32
    snprintf(cmd, sizeof(cmd),
        "powershell -Command \"Invoke-WebRequest -Uri '%s' -OutFile '%s'\"",
        url, output);
#else
    snprintf(cmd, sizeof(cmd),
        "wget -q -O \"%s\" \"%s\"",
        output, url);
#endif
    return system(cmd);
}

int sync_github_file() {
    MKDIR("data");

    if (download_file(REMOTE_URL, TEMP_PATH) != 0) {
        printf("❌ Download failed.\n");
        return 1;
    }

    if (!file_exists(LOCAL_PATH) || compare_files(LOCAL_PATH, TEMP_PATH)) {
        printf("Updating local file...\n");
        remove(LOCAL_PATH);
        rename(TEMP_PATH, LOCAL_PATH);
        printf("File updated.\n");
    } else {
        printf("Local file is up to date.\n");
        remove(TEMP_PATH);
    }

    return 0;
}