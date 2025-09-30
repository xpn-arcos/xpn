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

// Create a deep directory tree (e.g., /tmp/expand/P1/deep/level1/level2/.../levelN)
void test_deep_nesting(int levels) {
    char path[1024] = "/tmp/expand/P1/deep";
    int ret = mkdir(path, 0755);
    if (ret != 0) return;
    for (int i = 1; i <= levels; ++i) {
        snprintf(path + strlen(path), sizeof(path) - strlen(path), "/level%d", i);
        int ret = mkdir(path, 0755);
        char desc[2048];
        snprintf(desc, sizeof(desc), "Create deep nested dir '%s'", path);
        test_result(desc, 1, ret);
        if (ret != 0) break;
    }
}

// Create a wide directory tree (e.g., /tmp/expand/P1/wide/dir1 ... /tmp/expand/P1/wide/dirN)
void test_wide_fanout(int count) {
    char path[1024] = "/tmp/expand/P1/wide";
    int ret = mkdir(path, 0755);
    if (ret != 0) return;
    for (int i = 1; i <= count; ++i) {
        snprintf(path, sizeof(path), "/tmp/expand/P1/wide/dir%d", i);
        int ret = mkdir(path, 0755);
        char desc[2048];
        snprintf(desc, sizeof(desc), "Create wide dir '%s'", path);
        test_result(desc, 1, ret);
    }
}

// Stress test: create and remove directories in a loop
void test_stress_create_remove(int count) {
    char path[1024] = "/tmp/expand/P1/stress";
    int ret = mkdir(path, 0755);
    if (ret != 0) return;
    for (int i = 1; i <= count; ++i) {
        snprintf(path, sizeof(path), "/tmp/expand/P1/stress/dir%d", i);
        int ret = mkdir(path, 0755);
        char desc[2048];
        snprintf(desc, sizeof(desc), "Stress create dir '%s'", path);
        test_result(desc, 1, ret);
        if (ret == 0) {
            ret = rmdir(path);
            snprintf(desc, sizeof(desc), "Stress remove dir '%s'", path);
            test_result(desc, 1, ret);
        }
    }
}

int main(void)
{
    int ret;

    // 1. Create root directory
    ret = mkdir("/tmp/expand/P1/test_root", 0755);
    test_result("Create root directory 'test_root'", 1, ret);

    // 2. Create multiple subdirectories
    ret = mkdir("/tmp/expand/P1/test_root/sub1", 0755);
    test_result("Create 'test_root/sub1'", 1, ret);

    ret = mkdir("/tmp/expand/P1/test_root/sub2", 0755);
    test_result("Create 'test_root/sub2'", 1, ret);

    ret = mkdir("/tmp/expand/P1/test_root/sub3", 0755);
    test_result("Create 'test_root/sub3'", 1, ret);

    // 3. Try to create an existing directory
    ret = mkdir("/tmp/expand/P1/test_root/sub1", 0755);
    test_result("Try to create existing 'test_root/sub1'", 0, ret);

    // 4. Try to create directory in non-existent parent
    ret = mkdir("/tmp/expand/P1/doesnotexist/subx", 0755);
    test_result("Create dir in non-existent parent 'doesnotexist/subx'", 0, ret);

    // 5. Create a directory that will contain a file
    ret = mkdir("/tmp/expand/P1/with_file", 0755);
    test_result("Create 'with_file' directory", 1, ret);

    // 6. Create a file inside 'with_file'
    int fd = creat("/tmp/expand/P1/with_file/file1.txt", 0644);
    if (fd >= 0) {
        write(fd, "Hello POSIX!\n", 13);
        close(fd);
        printf("Test: Create file '/tmp/expand/P1/with_file/file1.txt'\n");
        printf("  Expected: Success\n");
        printf("  Got:      Success\n\n");
    } else {
        perror("open");
    }

    // 7. Large-scale directory creation tests
    test_deep_nesting(50);         // Deep nesting, e.g., 50 levels
    test_wide_fanout(100);         // Wide fan-out, e.g., 100 subdirs
    test_stress_create_remove(50); // Create and remove 50 dirs

    return 0;
}
