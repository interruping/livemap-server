//
//  dl_livemap_service_io_manager.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 5..
//
//

#include "dl_livemap_service_io_manager.hpp"

namespace solarcode {
namespace livemap {
    dl_livemap_service_io_manager::dl_livemap_service_io_manager(std::weak_ptr<session_base> session, client_node_pool& node_pool)
    :session_io_manager_base(session),
    _pimple(nullptr)
    {
        std::shared_ptr<session_base> target_session = session.lock();
        
        if ( target_session ) {
            target_session->set_delegate(this);
        }
    }
    void dl_livemap_service_io_manager::session_read_after_buffer(char *const buffer, const std::size_t buffer_length)
    {
        _pimple->session_read_after_buffer(buffer, buffer_length);
    }
    
    std::size_t dl_livemap_service_io_manager::session_write_before_buffer(char ** buffer)
    {
        return _pimple->session_write_before_buffer(buffer);
    }
    
}
}
