// Copyright 2017 GeunYoung Lim <interruping4dev@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
    
    std::size_t repeat_io_manager::session_write_before_buffer(char **buffer)
    {
        
        const std::size_t size_of_data = _repeatBuffer.size();
        *buffer = new char[size_of_data];
        std::memcpy(*buffer, _repeatBuffer.data(), size_of_data);
        return size_of_data;
    }
}
}
