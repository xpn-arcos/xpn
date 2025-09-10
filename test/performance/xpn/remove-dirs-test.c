#define _XOPEN_SOURCE 700


#include "all_system.h"
#include "xpn.h"

// Helper function to print test results
void test_result(const char *description, int expected_success, int ret) {
    printf("Test: %s\n", description);
    printf("  Expected: %s\n", expected_success ? "Success" : "Failure");
    printf("  Got:      %s\n\n", (ret == 0) ? "Success" : strerror(errno));
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

    return 0;
}