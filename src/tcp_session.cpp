#include "solarcode_tcp_session.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

namespace solarcode {
namespace livemap {
    void boost_secure_tcp_session::start()
    {
#ifdef _DEBUG_
            SC_DBGMSG("session start.");
#endif
        _io_service.post(
        _strand_for_session.wrap(
        boost::bind(&boost_secure_tcp_session::_unsafe_start, shared_from_this())    
        )    
        );
        
    }
    
    void boost_secure_tcp_session::stop()
    {
#ifdef _DEBUG_
            SC_DBGMSG("session stop session will expire.");
#endif

    }
    
    void boost_secure_tcp_session::set_expire_callback(std::function<void()> callback)
    {
#ifdef _DEBUG_
        SC_DBGMSG("set expire callback.");
#endif
        
        _expire_callback = callback;
        
        
    }
    
    void boost_secure_tcp_session::_unsafe_start()
    {
#ifdef _DEBUG_
        SC_DBGMSG("session start handshake process.");
#endif
        std::shared_ptr<ssl_socket_t> socket = get_socket();
        
        socket->async_handshake(boost::asio::ssl::stream_base::server,
        _strand_for_session.wrap(
        boost::bind(&boost_secure_tcp_session::handshake_complete, shared_from_this(),
          boost::asio::placeholders::error)
        )  
        );
        

    }
    
    void boost_secure_tcp_session::_unsafe_stop()
    {
#ifdef _DEBUG_
        SC_DBGMSG("session stop.");
#endif
        
    }
    
    void boost_secure_tcp_session::handshake_complete(const boost::system::error_code &error)
    {
#ifdef _DEBUG_
        SC_DBGMSG("handshake complete start socket read.");
#endif
        
        _io_trigger_timer.expires_from_now(
        boost::posix_time::milliseconds(100)
        );
        _io_trigger_timer.async_wait(
        _strand_for_session.wrap(
        boost::bind(&boost_secure_tcp_session::io_read_trigger, shared_from_this(), boost::asio::placeholders::error)    
        )
        );
    }
    
    void boost_secure_tcp_session::io_read_trigger(const boost::system::error_code& error)
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
            std::shared_ptr<ssl_socket_t> socket = get_socket();
            

            static const std::size_t buffer_length = 512;
            
            char tmp_raw_buffer[buffer_length] = {0,};

            boost::system::error_code error;
            std::size_t length = socket->read_some(boost::asio::buffer(tmp_raw_buffer), error);

            tcp_session_io_delegate *io_delegate = get_delegate();
            
            if ( io_delegate != nullptr ) {
                io_delegate->readbyte( tmp_raw_buffer, buffer_length);
            }
            
            _io_trigger_timer.async_wait(
            _strand_for_session.wrap(
            boost::bind(&boost_secure_tcp_session::io_write_trigger, shared_from_this(), boost::asio::placeholders::error)    
            )    
            );
        }
    }
    
    void boost_secure_tcp_session::io_write_trigger(const boost::system::error_code& error)
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
            std::shared_ptr<ssl_socket_t> socket = get_socket();
            
            static const std::size_t buffer_length = 512;
            
            char tmp_raw_buffer[buffer_length] = {0,};
            
            tcp_session_io_delegate *io_delegate = get_delegate();
            
            if ( io_delegate != nullptr ) {
                io_delegate->writebyte( tmp_raw_buffer, buffer_length);
            }
            
            socket->write_some(boost::asio::buffer(tmp_raw_buffer, buffer_length));
            
            _io_trigger_timer.async_wait(
            _strand_for_session.wrap(
            boost::bind(&boost_secure_tcp_session::io_read_trigger, shared_from_this(), boost::asio::placeholders::error)    
            )    
            );
        }
    }

}
}
