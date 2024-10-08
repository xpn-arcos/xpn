
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
#include <atomic>
#include <sstream>
#include <iomanip>

namespace XPN
{
    class xpn_stats
    {
    public:
        struct io_stats{
            uint64_t get_size() {return size;}
            uint64_t get_utime() {return utime;}
            uint64_t get_count() {return count;}
            double get_avg_size() {
                uint64_t guard = count;
                if (count == 0) guard = 1;
                return static_cast<double>(size) / static_cast<double>(guard);
            }
            double get_avg_utime() {
                uint64_t guard = count;
                if (count == 0) guard = 1;
                return static_cast<double>(utime) / static_cast<double>(guard);
            }
            double get_bandwidth() {
                double mb = static_cast<double>(get_size()) / MB;
                double sec = get_utime() / 1000 / 1000;
                if (sec == 0) sec = 1;
                return mb / sec;
            }
            void add_value(uint64_t add_size, uint64_t add_utime) {
                size+=add_size;
                utime+=add_utime;
                count+=1;
            }

            std::string to_string(){
                std::stringstream out;
                out << "Total : " << std::fixed << std::setprecision(2) << std::setw(8) << static_cast<double>(get_size())/(MB);
                out << " mb " << std::setw(8) << get_utime();
                out << " usec" << std::setw(8) << get_count();
                out << " count  Avg : " << std::fixed << std::setprecision(2) << std::setw(8) << get_avg_size()/(KB);
                out << " kb " << std::fixed << std::setprecision(2) << std::setw(8) << get_avg_utime();
                out << " usec " << std::fixed << std::setprecision(2) << std::setw(8) << get_bandwidth();
                out << " mb/sec ";
                return out.str();
            }
        private:
            std::atomic_uint64_t size = 0;
            std::atomic_uint64_t utime = 0;
            std::atomic_uint64_t count = 0;
        };

    public:
        io_stats read_disk;
        io_stats write_disk;
        io_stats read_net;
        io_stats write_net;

        class scope_stat
        {
        public:
            scope_stat(io_stats& stats, uint64_t value) : m_stats(stats), m_value(value) {}
            ~scope_stat() {m_stats.add_value(m_value, m_timer.elapsed<std::chrono::microseconds>());}
        private:
            io_stats& m_stats;
            uint64_t m_value;
            timer m_timer;
        };

        std::string to_string(){
            std::stringstream out;
            out << "Disk read  : " << read_disk.to_string() << std::endl;
            out << "Disk write : " << write_disk.to_string() << std::endl;
            out << "Net read   : " << read_net.to_string() << std::endl;
            out << "Net write  : " << write_net.to_string();
            return out.str();
        }
    };
}