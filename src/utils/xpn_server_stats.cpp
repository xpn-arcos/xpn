
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
#include <vector>
#include <string>
#include <thread>
#include <array>
#include "base_cpp/socket.hpp"
#include "base_cpp/timer.hpp"
#include "base_cpp/debug.hpp"
#include "xpn/xpn_stats.hpp"

using namespace XPN;

namespace actions
{
    constexpr const char * all = "all";
    constexpr const char * bw = "bw";
    constexpr const char * ops = "ops";
    constexpr const char * comb_bw = "comb_bw";
    constexpr const char * comb_ops = "comb_ops";
    constexpr const char * comb_all = "comb_all";
    
    constexpr const std::array<const char *, 6> list = {
        all,
        bw,
        ops,
        comb_bw,
        comb_ops,
        comb_all,
    };

    bool is_comb(const std::string& action){
        const std::string to_find = "comb";
        return action.find(to_find) != std::string::npos;
    }
    bool is_all(const std::string& action){
        const std::string to_find = "all";
        return action.find(to_find) != std::string::npos;
    }
} // namespace actions

void print_stats(std::string hostfile, std::string action)
{
    char srv_name[1024];
    FILE *file;

    debug_info("[TH_ID="<<std::this_thread::get_id()<<"] [XPN_SERVER] [xpn_server_print_stats] >> Begin");

    // Open host file
    debug_info("[TH_ID="<<std::this_thread::get_id()<<"] [XPN_SERVER] [xpn_server_print_stats] Open host file "<< m_params.shutdown_file);

    file = fopen(hostfile.c_str(), "r");
    if (file == NULL) {
        debug_error("[TH_ID="<<std::this_thread::get_id()<<"] [XPN_SERVER] [xpn_server_print_stats] ERROR: invalid file "<< m_params.shutdown_file);
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> srv_names;
    while (fscanf(file, "%[^\n] ", srv_name) != EOF)
    {
        srv_names.push_back(srv_name);
    }

    // Close host file
    debug_info("[TH_ID="<<std::this_thread::get_id()<<"] [XPN_SERVER] [xpn_server_print_stats] Close host file");

    fclose(file);

    std::cout << std::endl;
    
    double comb_bandwidth_disk_read = 0;
    double comb_bandwidth_disk_write = 0;
    double comb_bandwidth_net_read = 0;
    double comb_bandwidth_net_write = 0;
    double comb_bandwidth_total_read = 0;
    double comb_bandwidth_total_write = 0;
    std::array<double, static_cast<size_t>(xpn_server_ops::size)> comb_ops = {};

    for (auto &name : srv_names)
    {
        int socket;
        int ret;
        int buffer = socket::STATS_CODE;
        xpn_stats stat_buff;
        ret = socket::client_connect(name.data(), socket);
        if (ret < 0) {
            print("[TH_ID="<<std::this_thread::get_id()<<"] [XPN_SERVER] [xpn_server_print_stats] ERROR: socket connection " << name);
            continue;
        }

        ret = socket::send(socket, &buffer, sizeof(buffer));
        if (ret < 0) {
            print("[TH_ID="<<std::this_thread::get_id()<<"] [XPN_SERVER] [xpn_server_print_stats] ERROR: socket send " << name);
            continue;
        }
        
        ret = socket::recv(socket, &stat_buff, sizeof(stat_buff));
        if (ret < 0) {
            print("[TH_ID="<<std::this_thread::get_id()<<"] [XPN_SERVER] [xpn_server_print_stats] ERROR: socket recv " << name);
            continue;
        }
        socket::close(socket);

        if (!actions::is_comb(action)){
            std::cout << "Server " << name << ":" << std::endl;
            if (action == actions::bw || action == actions::all){
                std::cout << "Bandwidth :" << std::endl;
                std::cout << stat_buff.to_string_bandwidth() << std::endl;
            }
            if (action == actions::ops || action == actions::all){
                std::cout << "Ops :" << std::endl;
                std::cout << stat_buff.to_string_ops() << std::endl;
            }
            std::cout << std::endl;
        }

        comb_bandwidth_disk_read += stat_buff.m_read_disk.get_bandwidth();
        comb_bandwidth_disk_write += stat_buff.m_write_disk.get_bandwidth();
        comb_bandwidth_net_read += stat_buff.m_read_net.get_bandwidth();
        comb_bandwidth_net_write += stat_buff.m_write_net.get_bandwidth();
        comb_bandwidth_total_read += stat_buff.m_read_total.get_bandwidth();
        comb_bandwidth_total_write += stat_buff.m_write_total.get_bandwidth();
        for (size_t i = 0; i < comb_ops.size(); i++)
        {
            comb_ops[i] += stat_buff.m_ops_stats[i].get_ops_sec();
        }
    }

    if (action == actions::comb_bw || actions::is_all(action)){
        std::cout << "Combination bandwidth :" << std::endl;
        std::cout << std::setw(22) << "Disk read  " << " : " << std::fixed << std::setprecision(2) << std::setw(10) << comb_bandwidth_disk_read     << " mb/sec" << std::endl;
        std::cout << std::setw(22) << "Disk write " << " : " << std::fixed << std::setprecision(2) << std::setw(10) << comb_bandwidth_disk_write    << " mb/sec" << std::endl;
        std::cout << std::setw(22) << "Net read   " << " : " << std::fixed << std::setprecision(2) << std::setw(10) << comb_bandwidth_net_read      << " mb/sec" << std::endl;
        std::cout << std::setw(22) << "Net write  " << " : " << std::fixed << std::setprecision(2) << std::setw(10) << comb_bandwidth_net_write     << " mb/sec" << std::endl;
        std::cout << std::setw(22) << "Total read " << " : " << std::fixed << std::setprecision(2) << std::setw(10) << comb_bandwidth_total_read    << " mb/sec" << std::endl;
        std::cout << std::setw(22) << "Total write" << " : " << std::fixed << std::setprecision(2) << std::setw(10) << comb_bandwidth_total_write   << " mb/sec" << std::endl;
    }

    if (action == actions::comb_ops || actions::is_all(action)){
        std::cout << "Combination ops/sec :" << std::endl;
        for (size_t i = 0; i < comb_ops.size(); i++)
        {
            std::cout << std::setw(22) << xpn_server_ops_names[i] << " : " << std::fixed << std::setprecision(2) << std::setw(10) << comb_ops[i] << " ops/sec" << std::endl;
        }
        
    }

    debug_info("[TH_ID="<<std::this_thread::get_id()<<"] [XPN_SERVER] [xpn_server_up] >> End");
}

void show_usage(){
    
    std::cout << "Usage: xpn_server_stats <hostfile> <optional action:";
    for (size_t i = 0; i < actions::list.size(); i++)
    {
        std::cout << actions::list[i];
        if (i < actions::list.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ">" << std::endl;
}

// Main
int main ( int argc, char *argv[] )
{   
    std::string action = "all";

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    debug_info("[TH_ID="<<std::this_thread::get_id()<<"] [XPN_SERVER] [main] Get server params");

    if (argc < 2){
        show_usage();
    }

    if (argc >= 3){
        action = argv[2];
        auto it = std::find(actions::list.begin(), actions::list.end(), action);
        if (it == actions::list.end()){
            show_usage();
            exit(EXIT_FAILURE);
        }
    }

    print_stats(argv[1], action);

    return 0;
}

/* ................................................................... */

