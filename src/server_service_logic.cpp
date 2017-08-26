/*!
@file solarcode_server_service_logic.cpp
@breif  서버 서비스 로직 클래스 구현.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 02. 19
@version 0.0.1
*/
#include "server_service_logic.hpp"
#include "tcp_session.hpp"
#include "session_io_manager.hpp"

#include <mutex>
#include <iostream>

namespace solarcode {
namespace livemap {
    /*! @breif 서비스 시작 구현*/
    void livemap_server_service_logic::start_service ()
    {
#ifdef _DEBUG_
        SC_DBGMSG("service starting.");
#endif
        
        _server.set_delegate(this);
        _server.start_accept();
        
        for ( uint32_t count = 0; count < _num_of_thread; count++ ) {

            _thread_group.create_thread(
              boost::bind(&boost::asio::io_service::run, &_io_service)  
            );
        }

    }
    /*! @breif 서비스 중단 구현*/
    void livemap_server_service_logic::stop_service()
    {
#ifdef _DEBUG_
        SC_DBGMSG("service stoping.");
#endif
  
    }
    
    /*! @breif tcp_server_delegate 클래스 매서드 구현*/
    void livemap_server_service_logic::handle_accept(std::shared_ptr<ssl_socket_t> socket){
#ifdef _DEBUG_
        SC_DBGMSG("accept_handler called.");
#endif
    
        static std::mutex mutex_for_id_manger;
        
        std::unique_lock<std::mutex> id_work_lock(mutex_for_id_manger);
        
        common_id_type new_id = _id_manager.request_id();
        
        id_work_lock.unlock();
        
        if ( new_id == no_more_id ) {
            return;
        }
        
        std::shared_ptr<boost_secure_tcp_session> new_session =
        std::make_shared<boost_secure_tcp_session>(socket);
        
        livemap_session_io_manager *session_manager = new livemap_session_io_manager(_io_service, _query_interpreter);
        session_manager->set_id(new_id);
        session_manager->set_session_close_callback(
        [&, new_id](){
            std::unique_lock<std::mutex> id_return_lock(mutex_for_id_manger);
            _id_manager.return_id(new_id);
            id_return_lock.unlock();
        }    
        );

       _query_interpreter.register_delegate(new_id, session_manager);

       session_manager->start_managing_session(new_session);
        
    }
    /*! @breif tcp_server_delegate 클래스 매서드 구현*/
    void livemap_server_service_logic::error_occure(std::shared_ptr<ssl_socket_t> socket ,const boost::system::error_code& error){
#ifdef _DEBUG_
        SC_DBGMSG("error occure while accept proccess.");
#endif


    }
    
}
}
