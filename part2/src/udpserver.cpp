/**********************************
Copyright ACSDC/DEEC TECNICO LISBOA
alex@isr.ist.utl.pt
All rights reserved
***********************************/
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "threadhello.h"
#include "udpserver.h"
#include "nodeState.h"

using boost::asio::ip::udp;

udp_server::udp_server(boost::asio::io_service& io_service, int port_number,
                       nodeState& state)


    : socket_(io_service, udp::endpoint(udp::v4(), port_number)),
      state_(state)

{
   //start_receive();
}

void udp_server::start_receive()
{
    socket_.async_receive_from(
                               boost::asio::buffer(recv_buffer_),
                               remote_endpoint_,
                               // boost::bind(&udp_server::handle_receive, this,
                               //             boost::asio::placeholders::error));
                               boost::bind(&udp_server::handle_receive, this,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
                               // udp_server::handle_receive);
}



void udp_server::handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)

{
   if (!error || error == boost::asio::error::message_size)
   {
       // get string with the message
       std::string data = std::string(recv_buffer_.begin(),bytes_transferred);

       // get sender address
       std::string senderAdd = remote_endpoint_.address().to_string();
       // std::cout << "DATA RECEIVED FROM " << senderAdd << std::endl;
       // std::cout << "BYTES RECEIVED: " << bytes_transferred << "DATA: " << data << std::endl;

       int parseResult = state_.parseState(data,senderAdd);

       if (parseResult == -1 || parseResult == 0)
       {

           // parseResult returns -1 if it doesn't deal with the parsing automaticly

           boost::shared_ptr<std::string> message( new std::string(state_.micro_.getString()));
           socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                                 boost::bind(&udp_server::handle_send, this, message, boost::asio::placeholders::error));
       }

       if (parseResult == -1) {

           //modify the state of the environment according to the received information
           std::string data = std::string(recv_buffer_.begin(),recv_buffer_.end());
           std::istringstream is(data);
           int val;
           is >> std::hex >> val;
           // std::cout << "Data received converted  is " << val << std::endl;
           if(!is.fail())
           {
               state_.toWrite_=true;
               state_.send[0]=data[0]; state_.send[1]=data[1];
           }
        }

      start_receive();
   }
}

void udp_server::handle_send(boost::shared_ptr<std::string> msg, const boost::system::error_code& error)
{
}
