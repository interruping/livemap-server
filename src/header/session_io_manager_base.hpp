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
#include "client_node.hpp"
#include "debug_utility.hpp"

namespace solarcode {
namespace livemap {
    
    class session_io_manager_base : public session_io_delegate {
    public:
        session_io_manager_base(std::weak_ptr<session_base> session)
        :session_io_delegate(),
        _session(session),
        _owner(nullptr)
        {
            std::shared_ptr<session_base> target_session = session.lock();
            
            if ( target_session ) {
                target_session->set_delegate(this);
            }
        }
        
        virtual ~session_io_manager_base() {
#ifdef _DEBUG_
            SC_DBGMSG("session_io_manager_base destructor execute.");
#endif
        }
        
        virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length) = 0;
        virtual std::size_t session_write_before_buffer(char **buffer) = 0;
        
        virtual void set_session_owner(std::shared_ptr<client_node> owner) {
            _owner = owner;
        }
        
        virtual std::weak_ptr<client_node> get_session_owner() const {
            return _owner;
        }
        
        virtual std::weak_ptr<session_base> get_session() const
        {
            return _session;
        }
        
    private:
        
        std::weak_ptr<session_base> _session;
        std::shared_ptr<client_node> _owner;
    };
}
}
#endif /* session_io_manager_base_h */
