//
//  repeat_io_manager.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 31..
//
//

#include "repeat_io_manager.hpp"

namespace solarcode {
namespace livemap {
    void repeat_io_manager::session_read_after_buffer(char *const buffer, const std::size_t buffer_length){
        _repeatBuffer.clear();
        for ( std::size_t index = 0; index < buffer_length; index++){
            _repeatBuffer.push_back(buffer[index]);
        }
    }
    
    void repeat_io_manager::session_write_before_buffer(char *const buffer, const std::size_t buffer_length)
    {
        std::memcpy(buffer, _repeatBuffer.data(), buffer_length);
    }
}
}
