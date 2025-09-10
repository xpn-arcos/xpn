
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "all_system.h"
#include "xpn.h"

// Helper function to print test results
void test_result(const char *description, int expected_success, int ret) {
    printf("Test: %s\n", description);
    printf("  Expected: %s\n", expected_success ? "Success" : "Failure");
    printf("  Got:      %s\n\n", (ret == 0) ? "Success" : strerror(errno));
}

// Remove a deep directory tree (e.g., /P1/deep/level1/level2/.../levelN)
void test_remove_deep_nesting(int levels) {
    char path[1024];
    for (int i = levels; i >= 1; --i) {
        snprintf(path, sizeof(path), "/P1/deep");
        for (int j = 1; j <= i; ++j) {
            snprintf(path + strlen(path), sizeof(path) - strlen(path), "/level%d", j);
        }
        int ret = xpn_rmdir(path);
        char desc[2048];
        snprintf(desc, sizeof(desc), "Remove deep nested dir '%s'", path);
        test_result(desc, 1, ret);
    }
    // Remove the base directory
    int ret = xpn_rmdir("/P1/deep");
    test_result("Remove base dir '/P1/deep'", 1, ret);
}

// Remove a wide directory tree (e.g., /P1/wide/dir1 ... /P1/wide/dirN)
void test_remove_wide_fanout(int count) {
    char path[1024];
    for (int i = 1; i <= count; ++i) {
        snprintf(path, sizeof(path), "/P1/wide/dir%d", i);
        int ret = xpn_rmdir(path);
        char desc[2048];
        snprintf(desc, sizeof(desc), "Remove wide dir '%s'", path);
        test_result(desc, 1, ret);
    }
    // Remove the base directory
    int ret = xpn_rmdir("/P1/wide");
    test_result("Remove base dir '/P1/wide'", 1, ret);
}

// Stress test: create and remove directories in a loop
void test_stress_remove_create(int count) {
    char path[1024];
    for (int i = 1; i <= count; ++i) {
        snprintf(path, sizeof(path), "/P1/stress/dir%d", i);
        int ret = xpn_mkdir(path, 0755);
        if (ret == 0) {
            ret = xpn_rmdir(path);
            char desc[2048];
            snprintf(desc, sizeof(desc), "Stress remove dir '%s'", path);
            test_result(desc, 1, ret);
        }
    }
    // Remove the base directory
    int ret = xpn_rmdir("/P1/stress");
    test_result("Remove base dir '/P1/stress'", 1, ret);
}

int main(void) {
    int ret;

    // 1. Try to remove non-existent directory
    ret = xpn_rmdir("/P1/doesnotexist");
    test_result("Remove non-existent directory 'doesnotexist'", 0, ret);

    // 2. Remove some empty subdirectories
    ret = xpn_rmdir("/P1/test_root/sub2");
    test_result("Remove empty dir 'test_root/sub2'", 1, ret);

    ret = xpn_rmdir("/P1/test_root/sub3");
    test_result("Remove empty dir 'test_root/sub3'", 1, ret);

    // 3. Try to remove non-empty root
    ret = xpn_rmdir("/P1/test_root");
    test_result("Remove non-empty dir 'test_root'", 0, ret);

    // 4. Try to remove directory with a file inside
    ret = xpn_rmdir("/P1/with_file");
    test_result("Remove dir 'with_file' containing file", 0, ret);

    // 5. Remove the file first, then remove the directory
    ret = xpn_unlink("/P1/with_file/file1.txt");
    test_result("Remove file 'with_file/file1.txt'", 1, ret);

    ret = xpn_rmdir("/P1/with_file");
    test_result("Remove now-empty dir 'with_file'", 1, ret);

    // 6. Remove last subdir and then root
    ret = xpn_rmdir("/P1/test_root/sub1");
    test_result("Remove empty dir 'test_root/sub1'", 1, ret);

    ret = xpn_rmdir("/P1/test_root");
    test_result("Remove empty root dir 'test_root'", 1, ret);

    // 7. Large-scale directory removal tests
    test_remove_deep_nesting(50);      // Remove 50-level deep nested dirs
    test_remove_wide_fanout(100);      // Remove 100 sibling dirs
    test_stress_remove_create(50);     // Create and remove 50 dirs in a loop

    return 0;
}