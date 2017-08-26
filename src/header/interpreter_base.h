//
//  interpreter_base.h
//  livemap_server
//
//  Created by Geon young Lim on 2017. 8. 15..
//
//

#ifndef interpreter_base_h
#define interpreter_base_h

#include <cstdio>

namespace solarcode {
namespace livemap {
        /*!
         @class query_interpreter_delegate
         @breif 쿼리 해석 클래스 델리게이트 클래스.
         @detail n/a.
         @namespace solarcode::livemap::interpreter_base_delegate
         @see
         */
        template <typename user_id_type>
        class interpreter_base_delegate {
        public:
            virtual void complete_interprete(user_id_type user_id, const char *const query_result, std::size_t query_result_size) = 0;
        };
        
        /*!
         @class query_interpreter
         @breif 쿼리 해석 클래스.
         @detail n/a.
         @namespace solarcode::livemap::interpreter_base
         @see
         */
        template <typename user_id_type>
        class interpreter_base {
        public:
            virtual void request_interprete(user_id_type user_id, const char * const raw_query, const std::size_t raw_query_size) = 0;
            
            virtual void register_delegate(user_id_type delegate_id, interpreter_base_delegate<user_id_type> *delegate) = 0;
        };
}
}

#endif /* interpreter_base_h */
