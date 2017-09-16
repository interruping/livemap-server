//
//  dl_livemap_service_io_manager_boost_impl.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 5..
//
//

#include "dl_livemap_service_io_manager_boost_impl.hpp"
#include "command_binder.hpp"

namespace solarcode {
namespace livemap {
    dl_livemap_service_io_manager_boost_impl::dl_livemap_service_io_manager_boost_impl(std::weak_ptr<session_base> session, client_node_pool& node_pool)
    :solarcode::livemap::session_io_manager_base(session),
    _mutex_for_buffer(),
    _cv_for_buffer(),
    _io_service(get_shared_io_service()),
    _node_pool(node_pool),
    _buffer(nullptr),
    _buffer_size(0)
    {
        
        
        
    }
    
    
    boost::asio::strand
    dl_livemap_service_io_manager_boost_impl::_strand_for_node_pool
    = boost::asio::strand(get_shared_io_service());

    void dl_livemap_service_io_manager_boost_impl::session_read_after_buffer(char *const buffer, const std::size_t buffer_length)
    {
        std::vector<char> pass_buffer(buffer_length);
        for ( std::size_t index = 0; index < buffer_length; index++ ) {
            pass_buffer[0] = buffer[0];
        }
        
        auto unsafe_service = [this, pass_buffer, buffer_length]() {
            std::unique_lock<std::mutex> lock_for_buffer(_mutex_for_buffer);
   
            _buffer_size = command_bind(get_session_owner(),_node_pool, const_cast<char *>(pass_buffer.data()), buffer_length, &_buffer);
            //_cv_for_buffer.notify_one();
            
        };
        
        auto safe_service = _strand_for_node_pool.wrap(unsafe_service);
        _io_service.post(safe_service);
        
    }
    
    std::size_t dl_livemap_service_io_manager_boost_impl::session_write_before_buffer(char **buffer)
    {
        std::unique_lock<std::mutex> lock_for_buffer(_mutex_for_buffer);
        
        //_cv_for_buffer.wait(lock_for_buffer);
        //char *buffer_to_return = *buffer;
        
        *buffer = _buffer;
        
        _buffer = nullptr;
        std::size_t size_to_return = _buffer_size;
        _buffer_size = 0;
        
        return size_to_return;
    }
}
    
}
