
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

#include "workers_pool.hpp"

namespace XPN
{
    workers_pool::workers_pool() 
    {   
        m_num_threads = std::thread::hardware_concurrency() * 2;
        for (size_t i = 0; i < m_num_threads; ++i) { 
            m_threads.emplace_back([this] { 
                while (true) { 
                    std::function<void()> task; 
                    { 
                        std::unique_lock<std::mutex> lock(m_queue_mutex); 
                        
                        // Waiting until there is a task to 
                        // execute or the pool is stopped 
                        m_cv.wait(lock, [this] { 
                            return !m_tasks.empty() || m_stop; 
                        }); 
  
                        // exit the thread in case the pool 
                        // is stopped and there are no tasks 
                        if (m_stop && m_tasks.empty()) { 
                            return; 
                        } 
  
                        // Get the next task from the queue 
                        task = std::move(m_tasks.front()); 
                        m_tasks.pop(); 
                    } 
  
                    task(); 

                    {
                        std::unique_lock<std::mutex> lock(m_wait_mutex); 
                        m_wait--;
                        if (m_wait == 0){
                            m_wait_cv.notify_one();
                        }
                    }
                    m_full_cv.notify_one(); 
                } 
            }); 
        } 
    }
    
    workers_pool::~workers_pool() 
    {
        { 
            std::unique_lock<std::mutex> lock(m_queue_mutex); 
            m_stop = true; 
        } 
  
        m_cv.notify_all(); 
  
        for (auto& thread : m_threads) { 
            thread.join(); 
        } 
    }

    void workers_pool::launch(std::function<void()> task)
    {
        {
            std::unique_lock<std::mutex> lock(m_full_mutex);
            
            m_full_cv.wait(lock, [this] { 
                return m_tasks.size() <= m_num_threads; 
            }); 
        }
        {
            std::unique_lock<std::mutex> lock(m_queue_mutex);
            
            m_tasks.emplace(move(task)); 
        }
        {
            std::unique_lock<std::mutex> lock(m_wait_mutex);
            m_wait++;
        } 
        m_cv.notify_one(); 
    }

    void workers_pool::wait() 
    {
        std::unique_lock<std::mutex> lock(m_wait_mutex);
        
        m_wait_cv.wait(lock, [this] { 
            return m_wait == 0; 
        }); 
    }
} // namespace XPN