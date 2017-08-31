//
//  service_logic_base.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 30..
//
//

#ifndef service_logic_base_h
#define service_logic_base_h
#include "server_base.hpp"

#include <memory>

namespace solarcode {
namespace livemap {

    class service_logic_base : public server_service_delegate {
    public:
        virtual ~service_logic_base(){
            
        }
        
        virtual void set_target_server(server_base *target_server) = 0;
        virtual void handle_accept(std::shared_ptr<void> socket) = 0;
        virtual void error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error) = 0;
    protected:
        service_logic_base() {
            
        }
    };
}
}

#endif /* service_logic_base_h */
