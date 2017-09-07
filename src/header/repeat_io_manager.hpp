//
//  repeat_io_manager.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 31..
//
//

#ifndef repeat_io_manager_h
#define repeat_io_manager_h

#include <vector>

#include "session_io_manager_base.hpp"

namespace solarcode {
namespace livemap {
    class repeat_io_manager : public session_io_manager_base {
    public:
        repeat_io_manager(std::weak_ptr<session_base> session)
        :session_io_manager_base(session),
        _repeatBuffer()
        {

        }
        
        virtual ~repeat_io_manager() {};
        
        virtual void session_read_after_buffer(char *const buffer, const std::size_t buffer_length);
        virtual std::size_t session_write_before_buffer(char ** buffer);
        
    private:
        std::vector<char> _repeatBuffer;

    };
}
}

#endif /* repeat_io_manager_h */
