#define _XOPEN_SOURCE 700


#include "all_system.h"
#include "xpn.h"

// Helper function to print test results
void test_result(const char *description, int expected_success, int ret) {
    printf("Test: %s\n", description);
    printf("  Expected: %s\n", expected_success ? "Success" : "Failure");
    printf("  Got:      %s\n\n", (ret == 0) ? "Success" : strerror(errno));
}

int main(void) 
{
    int ret;

    // 1. Create root directory
    ret = xpn_mkdir("/P1/test_root", 0755);
    test_result("Create root directory 'test_root'", 1, ret);

    // 2. Create multiple subdirectories
    ret = xpn_mkdir("/P1/test_root/sub1", 0755);
    test_result("Create 'test_root/sub1'", 1, ret);

    ret = xpn_mkdir("/P1/test_root/sub2", 0755);
    test_result("Create 'test_root/sub2'", 1, ret);

    ret = xpn_mkdir("/P1/test_root/sub3", 0755);
    test_result("Create 'test_root/sub3'", 1, ret);

    // 3. Try to create an existing directory
    ret = xpn_mkdir("/P1/test_root/sub1", 0755);
    test_result("Try to create existing 'test_root/sub1'", 0, ret);

    // 4. Try to create directory in non-existent parent
    ret = xpn_mkdir("/P1/doesnotexist/subx", 0755);
    test_result("Create dir in non-existent parent 'doesnotexist/subx'", 0, ret);

    // 5. Create a directory that will contain a file
    ret = xpn_mkdir("/P1/with_file", 0755);
    test_result("Create 'with_file' directory", 1, ret);

    // 6. Create a file inside 'with_file'
    int fd = xpn_creat("/P1/with_file/file1.txt", 0644);
    if (fd >= 0) {
        xpn_write(fd, "Hello POSIX!\n", 13);
        xpn_close(fd);
        printf("Test: Create file '/P1/with_file/file1.txt'\n");
        printf("  Expected: Success\n");
        printf("  Got:      Success\n\n");
    } else {
        perror("open");
    }

    return 0;
}
