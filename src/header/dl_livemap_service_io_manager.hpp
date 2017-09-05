//
//  dl_livemap_service_io_manager.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 5..
//
//

#ifndef dl_livemap_service_io_manager_h
#define dl_livemap_service_io_manager_h

#include "session_io_manager_base.hpp"
#include "client_node_pool.hpp"

namespace solarcode {
namespace livemap {
    class dl_livemap_service_io_manager : public session_io_manager_base {
    public:
        dl_livemap_service_io_manager(std::weak_ptr<session_base> session, client_node_pool& node_pool);
        
        virtual ~dl_livemap_service_io_manager() {}
        
        virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length);
        virtual std::size_t session_write_before_buffer(char ** buffer);
        
    private:
        std::unique_ptr<session_io_manager_base> _pimple;
    };
}
}


#endif /* dl_livemap_service_io_manager_h */
