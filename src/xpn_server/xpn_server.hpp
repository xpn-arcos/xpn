
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

#include "xpn_server_params.hpp"
#include "xpn_server_comm.hpp"
#include "xpn_server_ops.hpp"
#include "base/workers.h"
#include <memory>


namespace XPN
{
    class xpn_server
    {
    public:
        xpn_server(int argc, char *argv[]);
        ~xpn_server();
        int run();
        int stop();

        void accept();
        static void dispatcher(struct st_th th);
        static void do_operation(struct st_th th);
        void finish();

        static xpn_server& Create(int argc, char *argv[]);

        static xpn_server& Get() { return *s_server; }

    public:
        char serv_name[HOST_NAME_MAX];
        xpn_server_params m_params;
        std::unique_ptr<xpn_server_control_comm> m_control_comm;
        worker_t m_worker1, m_worker2;

    private:
        static std::unique_ptr<xpn_server> s_server;

    public:
        
    // File operations
    static void op_open        ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_creat       ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_read        ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_write       ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_close       ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_rm          ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_rm_async    ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_rename      ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_setattr     ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_getattr     ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );

    // Directory operations
    static void op_mkdir       ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_opendir     ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_readdir     ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_closedir    ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_rmdir       ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_rmdir_async ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );

    // FS Operations
    static void op_getnodename ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_fstat       ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id ); //TODO: implement

    // Metadata
    static void op_read_mdata   ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_write_mdata  ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );
    static void op_write_mdata_file_size  ( xpn_server_comm &comm, struct st_xpn_server_msg &head, int rank_client_id, int tag_client_id );

    };    
}