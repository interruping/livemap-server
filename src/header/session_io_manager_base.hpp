//
//  session_io_manager_base.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 30..
//
//

#ifndef session_io_manager_base_h
#define session_io_manager_base_h

#include <memory>
#include "session_base.hpp"

namespace solarcode {
namespace livemap {
    
    class session_io_manager_base : public session_io_delegate {
    public:
        session_io_manager_base(std::weak_ptr<session_base> session)
        :session_io_delegate(),
        _session(session)
        {

        }
        
        virtual ~session_io_manager_base() {}
        
        virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length) = 0;
        virtual std::size_t session_write_before_buffer(char **buffer) = 0;
        
        virtual std::weak_ptr<session_base> get_session() const
        {
            return _session;
        }
        
    private:
        
        std::weak_ptr<session_base> _session;
        
    };
}
}
#endif /* session_io_manager_base_h */
