
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

#include "workers_sequential.hpp"

namespace XPN
{
    workers_sequential::workers_sequential() {}
    workers_sequential::~workers_sequential() {}

    std::future<int> workers_sequential::launch(std::function<int()> task)
    {
        std::packaged_task<int()> p_task(task);

        std::future<int> result = p_task.get_future();

        p_task();

        return result;
    }

    void workers_sequential::launch_no_future(std::function<void()> task)
    {
        task();
    }

    void workers_sequential::wait_all() {}
} // namespace XPN