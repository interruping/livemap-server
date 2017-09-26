#include "tcp_session.hpp"
#include "ssl_tcp_session_boost_impl.hpp"

namespace solarcode {
namespace livemap {

    ssl_tcp_session::ssl_tcp_session(std::shared_ptr<void> socket)
    :session_base(socket),
    _pimpl(new ssl_tcp_session_boost_impl(socket))
    {
        
    }
}
}
