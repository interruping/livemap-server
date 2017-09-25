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
            
            _io_trigger_timer.expires_from_now(boost::posix_time::milliseconds(1000));
            
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
                //SC_DBGMSG("error occure before socket read process.");
#endif
                
            } else {
                //정상 읽기 작업 수행
#ifdef _DEBUG_
                //SC_DBGMSG("read start socket read.");
#endif
                std::shared_ptr<bst_ssl_tcp_socket> socket = std::static_pointer_cast<bst_ssl_tcp_socket>(get_socket());
                
                char read_header_info[4] = {0,};
                std::size_t sizeof_int = 4;
                
                boost::system::error_code header_read_error;
                std::size_t length_of_header = socket->read_some(boost::asio::buffer(&read_header_info, sizeof_int), header_read_error);
                
                if ( length_of_header != sizeof_int ) {
                    //error;
                    return;
                }
                
                if ( header_read_error ) {
                    //error;
                    return;
                }
                
                
                int header_info = 0;
                
                std::memcpy(&header_info, read_header_info, sizeof_int);
                
                char *buffer_for_body = new char[header_info];
                
                boost::system::error_code body_error;
                std::size_t length_of_body = socket->read_some(boost::asio::buffer(buffer_for_body, header_info), body_error);
                
                
                
                
                session_io_delegate *io_delegate = get_delegate();
                
                if ( io_delegate != nullptr ) {
                    io_delegate->session_read_after_buffer(buffer_for_body, length_of_body);
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
                //SC_DBGMSG("error occure before write process.");
#endif
            } else {
                //정상 읽기 작업 수행
#ifdef _DEBUG_
                //SC_DBGMSG("start socket writes.");
#endif
                std::shared_ptr<bst_ssl_tcp_socket> socket = std::static_pointer_cast<bst_ssl_tcp_socket>(get_socket());
                
//                static const std::size_t buffer_length = 512;
//                
//                char tmp_raw_buffer[buffer_length] = {0,};
                
                session_io_delegate *io_delegate = get_delegate();
                
                std::size_t size_of_data_to_write = 0;
                
                char *buffer = nullptr;
                if ( io_delegate != nullptr ) {
                    size_of_data_to_write = io_delegate->session_write_before_buffer(&buffer);
                }

                
//                for (int index = 0; index < size_of_data_to_write; index++) {
//                    printf("\nbuffer to write: char[%d] = %2x", index, buffer[index]);
//                }
                
                int header_data = size_of_data_to_write;
                const int size_of_int = 4;
                boost::system::error_code header_error;
                
                boost::asio::write(*socket,boost::asio::buffer(&header_data, size_of_int),boost::asio::transfer_all(), header_error);
                
                boost::system::error_code error;

                boost::asio::write(*socket,boost::asio::buffer(buffer, size_of_data_to_write),boost::asio::transfer_all(), error);

                
                if ( buffer != nullptr ) {
                    delete[] buffer;
                }
                
                auto safe_wait_read
                = _strand_for_session.wrap(boost::bind(&ssl_tcp_session_boost_impl::io_read_trigger,
                                                       shared_from_this(),
                                                       boost::asio::placeholders::error));
                
                _io_trigger_timer.async_wait(safe_wait_read);
            }
        }
        
}
}

