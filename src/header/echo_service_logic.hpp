//
//  echo_service_logic.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 30..
//
//

#ifndef echo_service_logic_h
#define echo_service_logic_h

#include "service_logic_base.hpp"
#include <memory>

namespace solarcode {
namespace livemap {
    class echo_service_logic : public service_logic_base {
    public:
        echo_service_logic();
        
        virtual ~echo_service_logic() {
            
        }
        
        virtual void set_target_server(server_base *target_server);
        virtual void handle_accept(std::shared_ptr<void> socket);
        virtual void error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error);
    private:
        server_base::session_builder_type _session_builder;
    };
}
}

#endif /* echo_service_logic_h */
