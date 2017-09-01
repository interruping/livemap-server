//
//  livemap_service_logic.hpp
//  livemap_server
//
//  Created by Geon young Lim on 2017. 9. 1..
//
//

#ifndef livemap_service_logic_h
#define livemap_service_logic_h

#include "service_logic_base.hpp"


namespace solarcode {
namespace livemap {
    class livemap_service_logic : public service_logic_base {
    public:
        livemap_service_logic()
        :service_logic_base()
        {
            
        }
        
        virtual ~livemap_service_logic() {
            
        }
        
        virtual void set_target_server(server_base *target_server);
        virtual void handle_accept(std::shared_ptr<void> socket);
        virtual void error_occure(std::shared_ptr<void> socket, std::shared_ptr<void> error);
    private:
        static std::shared_ptr<
    };
}
}


#endif /* livemap_service_logic_h */
