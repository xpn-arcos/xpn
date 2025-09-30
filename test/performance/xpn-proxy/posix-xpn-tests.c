#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "all_system.h"

static void report_fail(const char * op,
    const char * path) {
    fprintf(stderr, "[FAIL] %s('%s') -> errno=%d (%s)\n\n", op, path ? path : "(null)", errno, strerror(errno));
}

static void report_ok(const char * op,
    const char * path) {
    printf("[ OK ] %s('%s')\n\n", op, path ? path : "(null)");
}

/* Helper to create a long repetitive name (not longer than max_len). */
static char * make_long_name(const char * prefix, size_t total_len) {
    if (!prefix) prefix = "file_";
    size_t pref_len = strlen(prefix);
    if (total_len <= pref_len) total_len = pref_len + 1;
    char * buf = malloc(total_len + 1);
    if (!buf) return NULL;
    strcpy(buf, prefix);
    size_t i = 0;
    i = pref_len;
    while (i < total_len) {
        buf[i++] = 'a' + (i % 23);
    }
    buf[total_len] = '\0';
    return buf;
}

static int test_file_ops(const char * base_dir,
    const char * name) {
    int ret = 0;
    char path[4096];
    snprintf(path, sizeof(path), "%s/%s", base_dir, name);

    /* 1) creat */
    printf("[INFO] Testing file operations on: %s\n", path);
    int fd = creat(path, 0644);
    if (fd == -1) {
        report_fail("creat", path);
        ret = -1;
        goto cleanup;
    }
    report_ok("creat", path);

    /* 2) write */
    const char * payload = "POSIX test payload\n";
    ssize_t w = write(fd, payload, strlen(payload));
    if (w != (ssize_t) strlen(payload)) {
        report_fail("write", path);
        ret = -1;
        close(fd);
        goto cleanup;
    }
    report_ok("write", path);

    /* 3) close */
    if (close(fd) == -1) {
        report_fail("close", path);
        ret = -1;
        goto cleanup;
    }
    report_ok("close", path);

    /* 4) stat */
    struct stat st;
    if (stat(path, & st) == -1) {
        report_fail("stat", path);
        ret = -1;
        goto cleanup;
    }
    report_ok("stat", path);

    /* 5) open */
    int rfd = open(path, O_RDONLY);
    if (rfd == -1) {
        report_fail("open", path);
        ret = -1;
        goto cleanup;
    }
    report_ok("open", path);

    /* 6) read */
    char buf[256];
    ssize_t r = read(rfd, buf, sizeof(buf) - 1);
    if (r <= 0) {
        report_fail("read", path);
        ret = -1;
        close(rfd);
        goto cleanup;
    }
    buf[r] = '\0';
    printf("[INFO] read %zd bytes: '%s'\n", r, buf);
    report_ok("read", path);

    /* 7) close */
    if (close(rfd) == -1) {
        report_fail("close", path);
        ret = -1;
        goto cleanup;
    }
    report_ok("close", path);

    /* 8) rename */
    char path2[4096];
    snprintf(path2, sizeof(path2), "%s/%s.renamed", base_dir, name);
    if (rename(path, path2) == -1) {
        report_fail("rename", path);
        ret = -1;
        goto cleanup;
    }
    report_ok("rename", path);

    /* 9) stat renamed */
    if (stat(path2, & st) == -1) {
        report_fail("stat", path2);
        ret = -1;
        goto cleanup;
    }
    report_ok("stat", path2);

    /* 10) unlink */
    if (unlink(path2) == -1) {
        report_fail("unlink", path2);
        ret = -1;
        goto cleanup;
    }
    report_ok("unlink", path2);

    cleanup:
        return ret;
}

static int test_dir_ops(const char * parent_dir,
    const char * dirname) {
    int ret = 0;
    char dirpath[4096];
    snprintf(dirpath, sizeof(dirpath), "%s/%s/", parent_dir, dirname);

    /* mkdir */
    if (mkdir(dirpath, 0755) == -1) {
        report_fail("mkdir", dirpath);
        return -1;
    }
    report_ok("mkdir", dirpath);

    /* opendir */
    DIR * d = opendir(dirpath);
    if (!d) {
        report_fail("opendir", dirpath);
        ret = -1;
        goto cleanup;
    }
    report_ok("opendir", dirpath);

    /* readdir */
    struct dirent * ent;
    int found = 0;
    while ((ent = readdir(d)) != NULL) {
        printf("[INFO] readdir: %s\n", ent -> d_name);
        found++;
    }
    if (found < 2) {
        fprintf(stderr, "[WARN] readdir found < 2 entries in %s\n", dirpath);
    }
    report_ok("readdir", dirpath);

    /* closedir */
    if (closedir(d) == -1) {
        report_fail("closedir", dirpath);
        ret = -1;
        goto cleanup;
    }
    report_ok("closedir", dirpath);

    /* rmdir */
    if (rmdir(dirpath) == -1) {
        report_fail("rmdir", dirpath);
        ret = -1;
        goto cleanup;
    }
    report_ok("rmdir", dirpath);

    cleanup:
        return ret;
}

int main(void) {
    int overall = 0;
    const char * tmp = "/P1";

    printf("=== TESTS WITH PATHS < 128 bytes ===\n");
    char base1[256];
    snprintf(base1, sizeof(base1), "%s/posix_test_short", tmp);
    if (mkdir(base1, 0755) == -1 && errno != EEXIST) {
        report_fail("mkdir", base1);
        return 2;
    }
    report_ok("mkdir", base1);

    char * short_name = make_long_name("short_", 50);
    if (!short_name) {
        fprintf(stderr, "malloc fail\n");
        return 2;
    }
    if (test_file_ops(base1, short_name) != 0) overall = 1;
    if (test_dir_ops(base1, "dshort") != 0) overall = 1;
    free(short_name);

    if (rmdir(base1) == -1) {
        if (errno == ENOENT) report_ok("rmdir", base1);
        else {
            printf("[INFO] trying cleanup of %s\n", base1);
        }
    } else report_ok("rmdir", base1);

    printf("\n=== TESTS WITH PATHS > 128 bytes ===\n");
    char base2[1024];
    snprintf(base2, sizeof(base2), "%s/posix_test_long", tmp);
    if (mkdir(base2, 0755) == -1 && errno != EEXIST) {
        report_fail("mkdir", base2);
        return 2;
    }
    report_ok("mkdir", base2);

    long name_max = pathconf("/", _PC_NAME_MAX);
    if (name_max <= 0) name_max = 255;
    printf("[INFO] NAME_MAX detected=%ld\n", name_max);

    size_t target_path_len = 300;
    size_t base2_len = strlen(base2);
    size_t need = 0;
    if (target_path_len > base2_len + 1) need = target_path_len - (base2_len + 1);
    size_t comp_len = (need < (size_t) name_max) ? need : (size_t) name_max - 1;
    if (comp_len < 10) comp_len = 10;

    char * long_dirname = make_long_name("Ldir_", comp_len);
    if (!long_dirname) {
        fprintf(stderr, "malloc fail\n");
        return 2;
    }

    char long_dirpath[2048];
    snprintf(long_dirpath, sizeof(long_dirpath), "%s/%s", base2, long_dirname);

    if (mkdir(long_dirpath, 0755) == -1) {
        report_fail("mkdir", long_dirpath);
        overall = 1;
    } else report_ok("mkdir", long_dirpath);

    char * final_name = make_long_name("longfile_", 40);
    if (!final_name) {
        fprintf(stderr, "malloc fail\n");
        return 2;
    }

    char fullpath[4000];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", long_dirpath, final_name);
    printf("[INFO] Long test full path length = %zu characters\n", strlen(fullpath));
    if (strlen(fullpath) <= 128) {
        printf("[WARN] generated path did not exceed 128 bytes. Consider increasing target_path_len.\n");
    }

    if (test_file_ops(long_dirpath, final_name) != 0) overall = 1;
    if (test_dir_ops(base2, long_dirname) != 0) overall = 1;

    if (rmdir(base2) == -1) {
        if (errno == ENOTEMPTY || errno == EEXIST || errno == EBUSY) {
            printf("[INFO] %s not empty, attempting cleanup...\n", base2);
            char rem[4096];
            snprintf(rem, sizeof(rem), "%s/%s.renamed", long_dirpath, final_name);
            unlink(rem);
            unlink(fullpath);
            rmdir(long_dirpath);
            if (rmdir(base2) == -1) report_fail("rmdir", base2);
            else report_ok("rmdir", base2);
        } else report_fail("rmdir", base2);
    } else report_ok("rmdir", base2);

    free(long_dirname);
    free(final_name);

    if (overall == 0) printf("\n=== RESULT: all tests passed (or errors were handled) ===\n");
    else printf("\n=== RESULT: some tests failed. Check error outputs. ===\n");

    return overall;
}
