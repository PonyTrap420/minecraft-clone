#include <shlobj.h>
#include <string.h>
#include <stdlib.h>
#include "miniz.h"
#include <tchar.h>
#include <direct.h> 

void get_minecraft_versions_path(char *out, size_t size) {
    char appdata[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appdata);
    snprintf(out, size, "%s\\.minecraft\\versions", appdata);
}

int find_latest_minecraft_jar(char *output_path, size_t size) {
    char versions_path[MAX_PATH];
    get_minecraft_versions_path(versions_path, sizeof(versions_path));

    WIN32_FIND_DATA fd;
    char search_path[MAX_PATH + 2];
    snprintf(search_path, sizeof(search_path), "%s\\*", versions_path);

    HANDLE hFind = FindFirstFile(search_path, &fd);
    FILETIME latest = {0};
    char latest_folder[MAX_PATH] = "";

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
            strcmp(fd.cFileName, ".") != 0 &&
            strcmp(fd.cFileName, "..") != 0) {

            char jar_path[MAX_PATH * 3 + 4];
            snprintf(jar_path, sizeof(jar_path), "%s\\%s\\%s.jar", versions_path, fd.cFileName, fd.cFileName);

            WIN32_FIND_DATA jar_fd;
            HANDLE hJar = FindFirstFile(jar_path, &jar_fd);
            if (hJar != INVALID_HANDLE_VALUE) {
                if (CompareFileTime(&jar_fd.ftLastWriteTime, &latest) > 0) {
                    latest = jar_fd.ftLastWriteTime;
                    snprintf(latest_folder, MAX_PATH, "%s", fd.cFileName);
                }
                FindClose(hJar);
            }
        }
    } while (FindNextFile(hFind, &fd));
    FindClose(hFind);

    if (latest_folder[0] == '\0') return 0; // Not found

    snprintf(output_path, size, "%s\\%s\\%s.jar", versions_path, latest_folder, latest_folder);
    return 1;
}

#define MAX_COMBINED_PATH (MAX_PATH * 2)

// Recursively create directories for given path
void ensure_directory_exists(const char *path) {
    char tmp[MAX_COMBINED_PATH];
    snprintf(tmp, sizeof(tmp), "%s", path);

    size_t len = strlen(tmp);
    if (tmp[len - 1] == '\\' || tmp[len - 1] == '/')
        tmp[len - 1] = '\0';

    for (char *p = tmp + 1; *p; ++p) {
        if (*p == '\\' || *p == '/') {
            *p = '\0';
            _mkdir(tmp); // Windows-specific
            *p = '\\';
        }
    }
    _mkdir(tmp);
}


int extract_minecraft_assets(const char *jar_path, const char* dest) {
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    if (!mz_zip_reader_init_file(&zip_archive, jar_path, 0)) {
        printf("Failed to open zip: %s\n", jar_path);
        return 0;
    }

    const char *match_prefix = "assets/minecraft/";
    size_t match_len = strlen(match_prefix);

    int file_count = (int)mz_zip_reader_get_num_files(&zip_archive);
    for (int i = 0; i < file_count; ++i) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) continue;

        if (strncmp(file_stat.m_filename, match_prefix, match_len) == 0) {
            const char *relative_path = file_stat.m_filename + match_len;
            char out_path[MAX_COMBINED_PATH];
            snprintf(out_path, sizeof(out_path), "%s\\%s", dest, relative_path);

            for (char *p = out_path; *p; ++p) {
                if (*p == '/') *p = '\\';
            }

            if (file_stat.m_is_directory) {
                ensure_directory_exists(out_path);
            } else {
                char dir_path[MAX_COMBINED_PATH];
                strcpy(dir_path, out_path);
                char *last_slash = strrchr(dir_path, '\\');
                if (last_slash) {
                    *last_slash = '\0';
                    ensure_directory_exists(dir_path);
                }

                if (!mz_zip_reader_extract_to_file(&zip_archive, i, out_path, 0)) {
                    printf("Failed to extract: %s\n", out_path);
                }
            }
        }
    }

    mz_zip_reader_end(&zip_archive);
    return 1;
}
