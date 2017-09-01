//
//  ssl_tcp_session_boost_impl.cpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 26..
//
//

#include "ssl_tcp_session_boost_impl.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

namespace solarcode {
namespace livemap {
    
        void ssl_tcp_session_boost_impl::start()
        {
#ifdef _DEBUG_
            SC_DBGMSG("session start.");
#endif
            auto safe_start
            = _strand_for_session.wrap(boost::bind(&ssl_tcp_session_boost_impl::_unsafe_start,
                                                   shared_from_this()));
            
            _io_service.post(safe_start);
            
        }
        
        void ssl_tcp_session_boost_impl::stop()
        {
#ifdef _DEBUG_
            SC_DBGMSG("session stop session will expire.");
#endif
            
        }
        
        void ssl_tcp_session_boost_impl::set_expire_callback(std::function<void()> callback)
        {
#ifdef _DEBUG_
            SC_DBGMSG("set expire callback.");
#endif
            
            _expire_callback = callback;
            
            
        }
        
        void ssl_tcp_session_boost_impl::_unsafe_start()
        {
#ifdef _DEBUG_
            SC_DBGMSG("session start handshake process.");
#endif
            std::shared_ptr<bst_ssl_tcp_socket> socket = std::static_pointer_cast<bst_ssl_tcp_socket>(get_socket());
            
            auto safe_handle_handshake
            = _strand_for_session.wrap(boost::bind(&ssl_tcp_session_boost_impl::handshake_complete,
                                                    shared_from_this(),
                                                    boost::asio::placeholders::error));
            
            socket->async_handshake(boost::asio::ssl::stream_base::server,safe_handle_handshake);
            
            
        }
        
        void ssl_tcp_session_boost_impl::_unsafe_stop()
        {
#ifdef _DEBUG_
            SC_DBGMSG("session stop.");
#endif
            
        }
        
        void ssl_tcp_session_boost_impl::handshake_complete(const boost::system::error_code &error)
        {
#ifdef _DEBUG_
            SC_DBGMSG("handshake complete start socket read.");
#endif
            
            _io_trigger_timer.expires_from_now(boost::posix_time::milliseconds(100));
            
            auto safe_wait_read
            = _strand_for_session.wrap(boost::bind(&ssl_tcp_session_boost_impl::io_read_trigger,
                                                   shared_from_this(),
                                                   boost::asio::placeholders::error));

            _io_trigger_timer.async_wait(safe_wait_read);
        }
        
        void ssl_tcp_session_boost_impl::io_read_trigger(const boost::system::error_code& error)
        {
            
            if ( error ) {
                //에러가 발생하였을 때 처리
#ifdef _DEBUG_
                SC_DBGMSG("error occure before socket read process.");
#endif
                
            } else {
                //정상 읽기 작업 수행
#ifdef _DEBUG_
                SC_DBGMSG("read start socket read.");
#endif
                std::shared_ptr<bst_ssl_tcp_socket> socket = std::static_pointer_cast<bst_ssl_tcp_socket>(get_socket());
                
                
                static const std::size_t buffer_length = 512;
                
                char tmp_raw_buffer[buffer_length] = {0,};
                
                boost::system::error_code error;
                std::size_t length = socket->read_some(boost::asio::buffer(tmp_raw_buffer, buffer_length), error);
                
                session_io_delegate *io_delegate = get_delegate();
                
                if ( io_delegate != nullptr ) {
                    io_delegate->session_read_after_buffer(tmp_raw_buffer, buffer_length);
                }
                
                auto safe_wait_write
                = _strand_for_session.wrap(boost::bind(&ssl_tcp_session_boost_impl::io_write_trigger,
                                                       shared_from_this(),
                                                       boost::asio::placeholders::error));
                
                _io_trigger_timer.async_wait(safe_wait_write);
            }
        }
        
        void ssl_tcp_session_boost_impl::io_write_trigger(const boost::system::error_code& error)
        {
            if ( error ) {
                //에러가 발생하였을 때 처리
#ifdef _DEBUG_
                SC_DBGMSG("error occure before write process.");
#endif
            } else {
                //정상 읽기 작업 수행
#ifdef _DEBUG_
                SC_DBGMSG("start socket writes.");
#endif
                std::shared_ptr<bst_ssl_tcp_socket> socket = std::static_pointer_cast<bst_ssl_tcp_socket>(get_socket());
                
                static const std::size_t buffer_length = 512;
                
                char tmp_raw_buffer[buffer_length] = {0,};
                
                session_io_delegate *io_delegate = get_delegate();
                
                if ( io_delegate != nullptr ) {
                    io_delegate->session_write_before_buffer( tmp_raw_buffer, buffer_length);
                }
                
                socket->write_some(boost::asio::buffer(tmp_raw_buffer, buffer_length));
                
                auto safe_wait_read
                = _strand_for_session.wrap(boost::bind(&ssl_tcp_session_boost_impl::io_read_trigger,
                                                       shared_from_this(),
                                                       boost::asio::placeholders::error));
                
                _io_trigger_timer.async_wait(safe_wait_read);
            }
        }
        
}
}

