
/*
 *  Copyright 2020-2024 Felix Garcia Carballeira, Diego Camarmas Alonso, Alejandro Calderon Mateos, Dario Muñoz Muñoz
 *
 *  This file is part of Expand.
 *
 *  Expand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Expand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Expand.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <unistd.h>

#include <array>
#include <string>
#include <thread>
#include <vector>

#include "base_cpp/debug.hpp"
#include "base_cpp/socket.hpp"
#include "base_cpp/timer.hpp"
#include "base_cpp/workers.hpp"
#include "nfi/nfi_server.hpp"
#include "xpn/xpn_conf.hpp"
#include "xpn/xpn_stats.hpp"

using namespace XPN;

constexpr const int monitor_port = 3457;

static std::unique_ptr<workers> worker;

void monitor_stats(std::filesystem::path csv_path) {
    // char srv_name[1024];
    // FILE *file;

    debug_info("[TH_ID=" << std::this_thread::get_id() << "] [XPN_SERVER] [xpn_server_print_stats] >> Begin");

    // Open host file
    debug_info("[TH_ID=" << std::this_thread::get_id() << "] [XPN_SERVER] [xpn_server_print_stats] Open config file ");

    xpn_conf conf;

    xpn_stats comb_stats;

    std::mutex comb_stats_mutex;

    for (auto& url : conf.partitions[0].server_urls) {
        nfi_parser parser(url);
        auto& name = parser.m_server;

        worker->launch_no_future([&name, &comb_stats_mutex, &comb_stats]() {
            int socket;
            int ret;
            int buffer = socket::STATS_wINDOW_CODE;
            xpn_stats stat_buff;
            ret = socket::client_connect(name, socket::get_xpn_port(), socket);
            if (ret < 0) {
                print("[TH_ID=" << std::this_thread::get_id()
                                << "] [XPN_SERVER] [xpn_server_print_stats] ERROR: socket connection " << name);
                return;
            }

            ret = socket::send(socket, &buffer, sizeof(buffer));
            if (ret < 0) {
                print("[TH_ID=" << std::this_thread::get_id()
                                << "] [XPN_SERVER] [xpn_server_print_stats] ERROR: socket send " << name);
                return;
            }

            ret = socket::recv(socket, &stat_buff, sizeof(stat_buff));
            if (ret < 0) {
                print("[TH_ID=" << std::this_thread::get_id()
                                << "] [XPN_SERVER] [xpn_server_print_stats] ERROR: socket recv " << name);
                return;
            }
            socket::close(socket);

            std::unique_lock<std::mutex> lock(comb_stats_mutex);
            comb_stats = comb_stats + stat_buff;
        });
    }

    worker->wait_all();

    std::ofstream csv_file(csv_path, std::ofstream::out | std::ofstream::app);
    if (csv_file.is_open()) {
        comb_stats.set_time(
            std::chrono::duration_cast<std::chrono::microseconds>(xpn_window_stats::window_time).count());
        csv_file << comb_stats.to_csv();
        csv_file.flush();
    }

    debug_info("[TH_ID=" << std::this_thread::get_id() << "] [XPN_SERVER] [xpn_server_up] >> End");
}

void show_usage() {
    std::cout << "Usage: xpn_server_monitor <(optional) action: stop>" << std::endl;
    std::cout << "       it is necessary to set XPN_CONF as the config file or use the default" << std::endl;
    std::cout << "       it is necessary to set XPN_STATS_DIR as the output directory" << std::endl;
}

void run(std::atomic_bool& to_stop) {
    std::filesystem::path csv_path = xpn_env::get_instance().xpn_stats_dir;
    csv_path.append("xpn_server_monitor.csv");
    std::ofstream csv_file(csv_path);
    if (csv_file.is_open()) {
        xpn_stats aux_stats;
        csv_file << aux_stats.to_csv_header();
        csv_file.flush();
    }

    worker = workers::Create(workers_mode::thread_pool);

    while (!to_stop.load()) {
        monitor_stats(csv_path);
        std::this_thread::sleep_for(xpn_window_stats::window_time);
    }
}

void stop() {
    int ret;
    int socket;
    auto name = ns::get_host_ip();
    ret = socket::client_connect(name, monitor_port, socket);
    if (ret < 0) {
        print("Warning: there are no xpn_server_monitor running at the moment");
        return;
    }

    ret = socket::close(socket);
    if (ret < 0) {
        print("[xpn_server_monitor] ERROR: socket close " << socket);
        return;
    }
}

// Main
int main(int argc, char* argv[]) {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    std::string stop_action = "stop";

    std::atomic_bool to_stop = false;

    if (argc > 1 && argv[1] == stop_action) {
        stop();
    } else {
        if (!xpn_env::get_instance().xpn_stats_dir) {
            print("Warning: not set XPN_STATS_DIR");
            show_usage();
            return 1;
        }

        std::thread([&to_stop, argv]() {
            int ret = 0;
            int socket;
            ret = socket::server_create(monitor_port, socket);
            if (ret < 0) {
                print("[xpn_server_monitor] ERROR: socket create port" << monitor_port);
                return;
            }
            int connect_socket;
            ret = socket::server_accept(socket, connect_socket);
            if (ret < 0) {
                print("[xpn_server_monitor] ERROR: socket accept port" << monitor_port);
                return;
            }
            to_stop = true;
            ret = socket::close(socket);
            if (ret < 0) {
                print("[xpn_server_monitor] ERROR: socket close " << socket);
                return;
            }
            ret = socket::close(connect_socket);
            if (ret < 0) {
                print("[xpn_server_monitor] ERROR: socket close " << connect_socket);
                return;
            }
        }).detach();
        run(to_stop);
    }

    return 0;
}

/* ................................................................... */
