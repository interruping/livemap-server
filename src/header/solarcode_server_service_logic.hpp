/*!
@file solarcode_livemap_server_application.hpp
@breif livemap 서버 어플리케이션 클래스.
@detail n/a.
@author GeunYoung Lim, interruping@naver.com
@data 2017. 02. 18
@version 0.0.1
*/
#ifndef SOLARCODE_LIVEMAP_SERVER_SERVIE_LOGIC
#define SOLARCODE_LIVEMAP_SERVER_SERVIE_LOGIC


#include <map>
#include <stdint.h>
#include <memory>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "solarcode_debug_utility.hpp"
#include "solarcode_tcp_server.hpp"
#include "solarcode_id_manager.hpp"
#include "solarcode_query_interpreter.hpp"

namespace solarcode {
namespace livemap {
    /*!
    @class service_option_base
    @breif 서버 구동 옵션 베이스 
    @detail n/a.
    @namespace solarcode::livemap::service_option_base
    @see 
    */
    template <typename option_key, typename option_value> 
    class service_option_base {
    public:
        /*!
        @breif 생성자
        */
        service_option_base():_option_map()
        {
            
        }
        
        
        /*! 
        @breif 옵션을 설정하는 함수
        @param key 옵션 키.
        @param value 옵션 키에 대한 값.
        @return bool 설정 값 주입 성공 여부.
        */
        virtual bool set_option(const option_key &key, const option_value &value) 
        {
            auto ret = _option_map.insert({key, value});
            
            return ret.second;
        }
        
        /*!
        @breir 옵션 값을 읽어오는 함수
        @param key 불러올 옵션 값의 키.
        @return option_value& 불러온 옵션 값.
        */
        virtual const option_value& get_option_value(const option_key &key) const 
        {
            auto iter = _option_map.find(key);
            
            if ( iter != _option_map.end() ){
                return iter->second;
            }
            return _option_map.end()->second;
        }
    private:
        /*!
        @breif 옵션 값을 저장하는 컨테이너.
        */
        std::map<option_key, option_value> _option_map;

    };
    
    template <typename service_option >
    class server_service_logic_base {
    public:
        /*!
        @brief 생성자
        @param option 서버 구동시 필요한 옵션
        */
        server_service_logic_base( service_option &option )
        :_service_option(option)
        {
#ifdef _DEBUG_
            SC_DBGMSG("server_service_logic_base create.");
#endif
        }
        /*! 
        @breif 서버 서비스 실행 함수
        */
        virtual void start_service() = 0;
        /*!
        @breif 서버 서비스 중지 함수 
        */
        virtual void stop_service() = 0;
        

    protected:
        /*!
        @breif 서버 옵션 맴버 객체
        */
        service_option &_service_option;
    };
    
    //livemap 서버 구동 옵션 열거 값
    typedef enum livemap_service_options {
        livemap_server_service_port, //서비스 포트
        livemap_server_multi_thread // 스레드 사용 옵션
    } livemap_service_options;
    
    //livemap 서버 옵션 alias
    using  livemap_service_setting =  service_option_base<livemap_service_options, uint32_t> ;
    
    //livemap 서비스 로직 베이스 alias
    using  livemap_service_logic = server_service_logic_base<livemap_service_setting>;
    
    /*!
    @class livemap_server_service_logic
    @breif 라이브맵 서버 로직 클래스
    @detail n/a.
    @namespace solarcode::livemap::livemap_server_service_logic
    @see 
    */
    class livemap_server_service_logic : public livemap_service_logic, public boost_secure_tcp_server_delegate
    {
    public:
        /*!
        @breif 생성자
        @param setting 서비스 서버의 구동을 위한 셋팅 값 객체.
        */
        livemap_server_service_logic( livemap_service_setting &setting )
        :livemap_service_logic(setting),
        _io_service(),
        _work(_io_service),
        _server(_io_service, setting.get_option_value(livemap_server_service_port)),
        _num_of_thread(0),
        _thread_group(),
        _id_manager(100),
		 _query_interpreter(_io_service)
        {
#ifdef _DEBUG_
            SC_DBGMSG("livemap_server_service_logic create.");
#endif
            //구동할 스레드 갯수 로드
            uint32_t thread_count = setting.get_option_value(livemap_server_multi_thread);
            
            if( thread_count <= 0 ){
                thread_count = 1;
            }
#ifdef _DEBUG_
            SC_DBGMSG("when debuging. server service logic operate with only single thread (forced single thread).");
            //디버그 할 땐 싱글 스레드로 강제.
            thread_count = 1;
#endif 
            _num_of_thread = thread_count;

        }
        /*! 
        @breif 소멸자
        */
        ~livemap_server_service_logic()
        {
            _thread_group.join_all();    
        }
        
        //override to livemap_server_logic
        virtual void start_service();
        virtual void stop_service();
        
        //interface inherit from boost_secure_tcp_server_delegate
        virtual void handle_accept(std::shared_ptr<ssl_socket_t> socket);
        virtual void error_occure(std::shared_ptr<ssl_socket_t> socket ,const boost::system::error_code& error);
    

    private:

        /*! 
        @breif 부스트 asio의  io_service 객체
        */
        boost::asio::io_service _io_service;
                /*! 
        @breif 부스트 asio의  io_service  work 객체
        */
        boost::asio::io_service::work _work;
        /*!
        @breif tcp 서버 객체
        */
        boost_secure_tcp_server _server;
        /*!
        @breif 구동 스레드 갯수
        */
        uint32_t _num_of_thread;
        /*!
        @breif 스레드 그룹 객체
        */
        boost::thread_group _thread_group;
        /*!
        @breif 아이디 발급 객체
        */
        local_id_manager _id_manager;
        /*!
         @breif 쿼리 해석 객체
        */
         livemap_query_interpreter _query_interpreter;

    };
}
}
#endif
