#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <climits>

#include "service_command.hpp"

namespace solarcode{
namespace livemap{

class client
{
public:
  client(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context,
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
    : socket_(io_service, context)
  {
    socket_.set_verify_mode(boost::asio::ssl::verify_peer);
    socket_.set_verify_callback(
        boost::bind(&client::verify_certificate, this, _1, _2));

    boost::asio::async_connect(socket_.lowest_layer(), endpoint_iterator,
        boost::bind(&client::handle_connect, this,
          boost::asio::placeholders::error));
  }

  bool verify_certificate(bool preverified,
      boost::asio::ssl::verify_context& ctx)
  {
    char subject_name[256];
    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
    std::cout << "Verifying " << subject_name << "\n";

    return preverified;
  }

  void handle_connect(const boost::system::error_code& error)
  {
    if (!error)
    {
      socket_.async_handshake(boost::asio::ssl::stream_base::client,
          boost::bind(&client::handle_handshake, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Connect failed: " << error.message() << "\n";
    }
  }

  void handle_handshake(const boost::system::error_code& error)
  {
    if (!error)
    {

        test_command();

    }
    else
    {
      std::cout << "Handshake failed: " << error.message() << "\n";
    }
  }

    void test_command ()
    {
        std::cout << "Start test." << std::endl;
        //request_user_info test
        {
            request_user_info request_user_info_cmd;
            
            const int alloc_size = request_user_info_cmd.get_entire_size() + 4;
            char *buffer = new char[alloc_size];
            int type = request_user_info::type;
            
            std::memcpy(buffer, &type, 4);
            
            boost::system::error_code error;
            
            int header =  alloc_size;
            
            boost::asio::write(socket_, boost::asio::buffer(&header, 4),boost::asio::transfer_all(), error);
            error_handle(error);
            
            request_user_info_cmd.serialize(buffer + 4);
            
            boost::asio::write(socket_, boost::asio::buffer(buffer, header), boost::asio::transfer_all(), error);
            error_handle(error);
        
            int read_header;
            
            socket_.read_some(boost::asio::buffer(&read_header, 4), error);
            error_handle(error);
            
            char *read_buffer = new char[read_header];
            
            socket_.read_some(boost::asio::buffer(read_buffer, read_header), error);
            error_handle(error);
            
            int read_type = 0;
            
            std::memcpy(&read_type, read_buffer, 4);
            
            bool isFail = true;
            
            set_user_info set_user_info_cmd(read_buffer + 4, read_header - 4);
            
            if ( read_type == set_user_info::type ) {

            if ( set_user_info_cmd.get_id() < INT_MAX ){
                 std::cout << "Test request_user_info service Ok." << std::endl;
                isFail = false;
            }
               
            }
            
            if ( isFail ){
                std::cout << "Test request_user_info service Fail." << std::endl;
            }
            
        }
        //user_update_node 
        {
            client_node test_node(0);
            
            
            user_update_node user_update_node_cmd(test_node);
            
            const int alloc_size = user_update_node_cmd.get_entire_size() + 4;
            char *buffer = new char[alloc_size];
            int type = user_update_node::type;
            
            std::memcpy(buffer, &type, 4);
            
            boost::system::error_code error;
            
            int header =  alloc_size;
            
            boost::asio::write(socket_, boost::asio::buffer(&header, 4),boost::asio::transfer_all(), error);
            error_handle(error);
            
            user_update_node_cmd.serialize(buffer + 4);
            
            boost::asio::write(socket_, boost::asio::buffer(buffer, header), boost::asio::transfer_all(), error);
            error_handle(error);
            
            int read_header;
            
            socket_.read_some(boost::asio::buffer(&read_header, 4), error);
            error_handle(error);
            
            char *read_buffer = new char[read_header];
            
            socket_.read_some(boost::asio::buffer(read_buffer, read_header), error);
            error_handle(error);
            
            int read_type = 0;
            
            std::memcpy(&read_type, read_buffer, 4);
            
            bool isFail = true;
            
            near_node_info near_node_info_cmd(read_buffer + 4, read_header - 4);
            
            if ( read_type == near_node_info::type ){
                
            if ( near_node_info_cmd.get_near_nodes().size() == 0 ){
                std::cout << "Test user_update_node service Ok." << std::endl;
                isFail = false;
            }
                
            }
            
            if ( isFail ){
                std::cout << "Test user_update_node service Fail." << std::endl;
            }
            
        }
        {
            client_node test_node(0);
            
            std::string str = "1234";
            
            utf8_message_send utf8_message_send_cmd(0, 0, str);
            
            const int alloc_size = utf8_message_send_cmd.get_entire_size() + 4;
            char *buffer = new char[alloc_size];
            int type = utf8_message_send::type;
            
            std::memcpy(buffer, &type, 4);
            
            boost::system::error_code error;
            
            int header =  alloc_size;
            
            boost::asio::write(socket_, boost::asio::buffer(&header, 4),boost::asio::transfer_all(), error);
            error_handle(error);
            
            utf8_message_send_cmd.serialize(buffer + 4);
            
            boost::asio::write(socket_, boost::asio::buffer(buffer, header), boost::asio::transfer_all(), error);
            error_handle(error);
            
            int read_header;
            
            socket_.read_some(boost::asio::buffer(&read_header, 4), error);
            error_handle(error);
            
            char *read_buffer = new char[read_header];
            
            socket_.read_some(boost::asio::buffer(read_buffer, read_header), error);
            error_handle(error);
            
            int read_type = 0;
            
            std::memcpy(&read_type, read_buffer, 4);
            
            bool isFail = true;
            
            command_form_base command_form_base_cmd(read_buffer + 4, read_header - 4);
            
            if ( read_type == command_form_base::type ){

                std::cout << "utf8_message_send service Ok." << std::endl;
                isFail = false;

                
            }
            
            if ( isFail ){
                std::cout << "utf8_message_send service Fail." << std::endl;
            }
            
        }

        std::cout << "End Test." << std::endl;

    }

    void error_handle(boost::system::error_code error)
    {
        
        if ( error ) {
            std::cout << "Test Fail Error :" << error.message() << std::endl;
            
        }
        
    }
private:
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;

};


}
}
