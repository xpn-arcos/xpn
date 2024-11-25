
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

#pragma once

#include "base_cpp/timer.hpp"
#include "base_cpp/ns.hpp"
#include "xpn_server/xpn_server_params.hpp"
#include "xpn_server/xpn_server_ops.hpp"
#include <atomic>
#include <sstream>
#include <iomanip>
#include <array>
#include <type_traits>
#include <fstream>
#include <filesystem>

namespace XPN
{
    class xpn_stats
    {
    private:
        struct stats{
            stats() = default;
            stats(const stats& other) {
                m_utime = other.m_utime.load();
                m_count = other.m_count.load();
            }
            stats& operator=(const stats& other) {
                if (this != &other) {
                    m_utime = other.m_utime.load();
                    m_count = other.m_count.load();
                }
                return *this;
            }

            uint64_t get_utime() {return m_utime;}
            uint64_t get_count() {return m_count;}
            double get_avg_utime() {
                uint64_t guard = m_count;
                if (m_count == 0) guard = 1;
                return static_cast<double>(m_utime) / static_cast<double>(guard);
            }
            double get_ops_sec() {
                double sec = static_cast<double>(get_utime()) / 1000 / 1000;
                if (sec == 0) sec = 1;
                return m_count / sec;
            }
            void add_value() {
                m_count+=1;
            }
            void add_time(uint64_t add_utime) {
                m_utime+=add_utime;
            }
            void set_time(uint64_t utime){
                m_utime = utime;
            }
            
        protected:
            std::atomic_uint64_t m_utime = 0;
            std::atomic_uint64_t m_count = 0;
        };
    public:
        struct io_stats : public stats{
            io_stats() = default;
            io_stats(const io_stats& other) : stats(other) {
                m_count = other.m_count.load();
            }
            
            io_stats& operator=(const io_stats& other) {
                if (this != &other) {
                    m_utime = other.m_utime.load();
                    m_size = other.m_size.load();
                    m_count = other.m_count.load();
                }
                return *this;
            }

            uint64_t get_size() {return m_size;}
            double get_avg_size() {
                uint64_t guard = m_count;
                if (m_count == 0) guard = 1;
                return static_cast<double>(m_size) / static_cast<double>(guard);
            }
            double get_bandwidth() {
                double mb = static_cast<double>(get_size()) / MB;
                double sec = static_cast<double>(get_utime()) / 1000 / 1000;
                if (sec == 0) sec = 1;
                return mb / sec;
            }
            void add_value(uint64_t add_size) {
                m_size+=add_size;
                m_count+=1;
            }

            io_stats operator-(const io_stats& other) const {
                io_stats out;
                out.m_utime = m_utime - other.m_utime;
                out.m_size = m_size - other.m_size;
                out.m_count = m_count - other.m_count;
                return out;
            }

            io_stats operator+(const io_stats& other) const {
                io_stats out;
                out.m_utime = m_utime + other.m_utime;
                out.m_size = m_size + other.m_size;
                out.m_count = m_count + other.m_count;
                return out;
            }

            std::string to_string(){
                std::stringstream out;
                out << " | Total | "<< std::fixed << std::setprecision(2) << std::setw(10) << static_cast<double>(get_size())/(MB)  << " mb";
                out << " | " <<                                              std::setw(10) << get_utime() / 1000                    << " msec";
                out << " | " <<                                              std::setw(10) << get_count()                           << " count";
                out << " | Avg | "  << std::fixed << std::setprecision(2) << std::setw(10) << get_avg_size()/(KB)                   << " kb";
                out << " | " <<        std::fixed << std::setprecision(2) << std::setw(10) << get_avg_utime()                       << " usec";
                out << " | " <<        std::fixed << std::setprecision(2) << std::setw(10) << get_bandwidth()                       << " mb/sec";
                out << " | ";
                return out.str();
            }
        private:
            std::atomic_uint64_t m_size = 0;
        };

        struct op_stats : public stats{
            
            op_stats operator-(const op_stats& other) const {
                op_stats out;
                out.m_utime = m_utime - other.m_utime;
                out.m_count = m_count - other.m_count;
                out.m_op = m_op;
                return out;
            }

            op_stats operator+(const op_stats& other) const {
                op_stats out;
                out.m_utime = m_utime + other.m_utime;
                out.m_count = m_count + other.m_count;
                out.m_op = m_op;
                return out;
            }

            std::string to_string(){
                std::stringstream out;
                out << std::setw(22) << xpn_server_ops_name(m_op);
                out << " | Total | "<<                                       std::setw(10) << get_utime() / 1000                    << " msec";
                out << " | " <<                                              std::setw(10) << get_count()                           << " count";
                out << " | Avg | "<<   std::fixed << std::setprecision(2) << std::setw(10) << get_avg_utime()                       << " usec";
                out << " | " <<        std::fixed << std::setprecision(2) << std::setw(10) << get_ops_sec()                       << " ops/sec";
                out << " | ";
                return out.str();
            }
            xpn_server_ops m_op = xpn_server_ops::size;
        };


    public:
        xpn_stats()
        {
            for (size_t i = 0; i < m_ops_stats.size(); i++)
            {
                m_ops_stats[i].m_op = static_cast<xpn_server_ops>(i);
            }
        }

    public:
        io_stats m_read_disk;
        io_stats m_write_disk;
        io_stats m_read_net;
        io_stats m_write_net;
        io_stats m_read_total;
        io_stats m_write_total;

        std::array<op_stats, static_cast<size_t>(xpn_server_ops::size)> m_ops_stats;

        template<typename stat_type>
        class scope_stat
        {
        public:
            scope_stat(stat_type& stats, uint64_t value, timer t = timer()) : m_timer(t), m_stats(stats), m_value(value) {
            }

            scope_stat(stat_type& stats, timer t = timer()) : m_timer(t), m_stats(stats) {
            }
            
            ~scope_stat(){
                auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_timer.get_start()).count();
                
                if constexpr (std::is_same_v<stat_type, io_stats>){
                    m_stats.add_value(m_value);
                }else if constexpr (std::is_same_v<stat_type, op_stats>){
                    m_stats.add_value();
                }
                m_stats.add_time(elapsed_time);
            }
        protected:
            timer m_timer;
        private:
            stat_type& m_stats;
            uint64_t m_value = 0;
        };

        xpn_stats operator-(const xpn_stats& other) const {
            xpn_stats out;
            out.m_read_disk = m_read_disk - other.m_read_disk;
            out.m_write_disk = m_write_disk - other.m_write_disk;
            out.m_read_net = m_read_net - other.m_read_net;
            out.m_write_net = m_write_net - other.m_write_net;
            out.m_read_total = m_read_total - other.m_read_total;
            out.m_write_total = m_write_total - other.m_write_total;

            for (size_t i = 0; i < m_ops_stats.size(); i++)
            {
                out.m_ops_stats[i] = m_ops_stats[i] - other.m_ops_stats[i]; 
            }
            return out;
        }
        
        xpn_stats operator+(const xpn_stats& other) const {
            xpn_stats out;
            out.m_read_disk = m_read_disk + other.m_read_disk;
            out.m_write_disk = m_write_disk + other.m_write_disk;
            out.m_read_net = m_read_net + other.m_read_net;
            out.m_write_net = m_write_net + other.m_write_net;
            out.m_read_total = m_read_total + other.m_read_total;
            out.m_write_total = m_write_total + other.m_write_total;

            for (size_t i = 0; i < m_ops_stats.size(); i++)
            {
                out.m_ops_stats[i] = m_ops_stats[i] + other.m_ops_stats[i]; 
            }
            return out;
        }

        void set_time(const uint64_t& utime){
            
            m_read_disk.set_time(utime);
            m_write_disk.set_time(utime);
            m_read_net.set_time(utime);
            m_write_net.set_time(utime);
            m_read_total.set_time(utime);
            m_write_total.set_time(utime);

            for (size_t i = 0; i < m_ops_stats.size(); i++)
            {
                m_ops_stats[i].set_time(utime); 
            }
        }

        std::string to_string_bandwidth(){
            std::stringstream out;
            out << "Disk read   " << m_read_disk.to_string() << std::endl;
            out << "Disk write  " << m_write_disk.to_string() << std::endl;
            out << "Net read    " << m_read_net.to_string() << std::endl;
            out << "Net write   " << m_write_net.to_string() << std::endl;
            out << "Total read  " << m_read_total.to_string() << std::endl;
            out << "Total write " << m_write_total.to_string();
            return out.str();
        }
        
        std::string to_string_ops(){
            std::stringstream out;
            for (auto &op : m_ops_stats)
            {
                out << "OP: " << op.to_string() << std::endl;
            }
            return out.str();
        }

        std::string to_csv_header(){
            std::stringstream out;
            out << "Timestamp" << ";";
            out << "Read (mb/sec)" << ";";
            out << "Read (mb)" << ";";
            out << "Avg read (kb)" << ";";
            out << "Write (mb/sec)" << ";";
            out << "Write (mb)" << ";";
            out << "Avg write (kb)" << ";";
            
            for (auto &op : m_ops_stats)
            {
                out << "OP " << xpn_server_ops_name(op.m_op) << " (ops/sec);";
            }

            out << std::endl;
            return out.str();
        }

        std::string to_csv(){
            std::stringstream out;
            std::stringstream out_data;
            auto now = std::chrono::high_resolution_clock::now();
            std::time_t actual_time = std::chrono::high_resolution_clock::to_time_t(now);
            std::tm formated_time = *std::localtime(&actual_time);
            auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
            out << std::put_time(&formated_time, "%Y-%m-%d %H:%M:%S") << "," << std::setw(3) << std::setfill('0') << millisec.count() << ";";

            out_data << m_read_total.get_bandwidth() << ";";
            out_data << m_read_total.get_size() / 1024 / 1024 << ";";
            out_data << m_read_total.get_avg_size() / 1024 << ";";
            out_data << m_write_total.get_bandwidth() << ";";
            out_data << m_write_total.get_size() / 1024 / 1024 << ";";
            out_data << m_write_total.get_avg_size() / 1024 << ";";
            
            for (auto &op : m_ops_stats)
            {
                out_data << op.get_ops_sec() << ";";
            }

            std::string replace_point = out_data.str();
            std::replace(replace_point.begin(), replace_point.end(), '.', ',');

            out << replace_point;
            out << std::endl;
            return out.str();
        }
    };

    class xpn_window_stats{
    public:
        xpn_window_stats(xpn_stats& stats) : m_stats(stats) 
        {
            m_thread = std::thread(&xpn_window_stats::thread_xpn_stats, this);

            if(xpn_env::get_instance().xpn_stats_dir){
                std::filesystem::path csv_path = xpn_env::get_instance().xpn_stats_dir;
                csv_path.append(ns::get_host_name()+".csv");
                csv_file = std::ofstream(csv_path);
                if (csv_file.is_open()){
                    csv_file << m_stats.to_csv_header();
                }
            }
        }

        ~xpn_window_stats()
        {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                is_running = false;
            }
            m_cv.notify_one();
            m_thread.join();
        }

        xpn_stats& get_current_stats() { return m_window_stats[m_actual_index]; }

        constexpr static const int window_size = 10;
        constexpr static const std::chrono::duration window_time = std::chrono::milliseconds(500);

    private:
        std::array<xpn_stats, window_size> m_window_stats;
        std::atomic_bool is_running = true;
        std::mutex m_mutex;
        std::condition_variable m_cv;

        std::thread m_thread;

        xpn_stats& m_stats;

        xpn_stats m_previous;
        int m_actual_index = 0;

        std::ofstream csv_file;

        void thread_xpn_stats(){
            std::unique_lock<std::mutex> lock(m_mutex);
            while (is_running) {
                if (m_cv.wait_for(lock, window_time, [this]{ return !is_running; })) {
                    break;
                }

                m_actual_index = (m_actual_index + 1) % window_size;
                m_window_stats[m_actual_index] = m_stats - m_previous;
                m_window_stats[m_actual_index].set_time(std::chrono::duration_cast<std::chrono::microseconds>(window_time).count());
                m_previous = m_stats;

                if (csv_file.is_open()){
                    csv_file << m_window_stats[m_actual_index].to_csv();
                    csv_file.flush();
                }
                // std::cout << "Actual index " << m_actual_index << std::endl;

                // std::cout << m_window_stats[m_actual_index].to_string_bandwidth() << std::endl;
                // std::cout << m_window_stats[m_actual_index].to_string_ops() << std::endl;

            }
        }
    };
}