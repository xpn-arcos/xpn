
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

#include "workers.hpp"
#include <condition_variable> 
#include <mutex> 
#include <queue> 
#include <thread> 
#include <variant> 

namespace XPN
{
    class workers_pool : public workers
    {
    public:
        workers_pool();
        ~workers_pool();

        std::future<int> launch(std::function<int()> task) override;
        void launch_no_future(std::function<void()> task) override;
        void wait_all() override;
    private:
        std::vector<std::thread> m_threads;
        std::queue<std::variant<std::packaged_task<int()>,std::function<void()>>> m_tasks;
        std::mutex m_queue_mutex;
        std::condition_variable m_cv;

        std::mutex m_wait_mutex;
        std::condition_variable m_wait_cv;

        std::mutex m_full_mutex;
        std::condition_variable m_full_cv;

        bool m_stop = false;
        int m_wait = 0;

        size_t m_num_threads = 0;
    };
} // namespace XPN
