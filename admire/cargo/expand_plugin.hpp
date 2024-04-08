
#ifndef EXPAND_PLUGIN_HPP
#define EXPAND_PLUGIN_HPP

#include "fs_plugin.hpp"

namespace cargo {
class expand_plugin : public FSPlugin {

public:
    expand_plugin();
    ~expand_plugin();
    int
    open(const std::string& path, int flags, unsigned int mode) final;
    bool
    close(int fd) final;
    ssize_t
    pread(int fd, void* buf, size_t count, off_t offset) final;
    ssize_t
    pwrite(int fd, const void* buf, size_t count, off_t offset) final;
    bool
    mkdir(const std::string& path, mode_t mode) final;
    off_t
    lseek(int fd, off_t offset, int whence) final;
    off_t
    fallocate(int fd, int mode, off_t offset, off_t len) final;
};
}; // namespace cargo

#endif // EXPAND_PLUGIN_HPP
