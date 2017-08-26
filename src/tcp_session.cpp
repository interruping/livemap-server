#include "tcp_session.hpp"

namespace solarcode {
namespace livemap {

    ssl_tcp_session::ssl_tcp_session(std::shared_ptr<void> socket)
    :session_base(socket),
    _pimple(nullptr)
    {
        
    }
}
}
