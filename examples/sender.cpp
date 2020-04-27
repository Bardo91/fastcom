#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <functional>
#include <thread>
#include <set>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::placeholders::_3;
using websocketpp::lib::bind;


#include <fastcom/Publisher.h>

class ServerTest{
public:
    void init(int _port){
        std::cout << "Getting ready server on port " << _port << std::endl;
        this->magic(_port);
    }

    void send(std::string _msg){

        websocketpp::lib::error_code ec;
        lock.lock();
        for(auto con:m_connections){
            echo_server.send(con, _msg, websocketpp::frame::opcode::text, ec);
        }
        lock.unlock();
    }

private:
    typedef websocketpp::server<websocketpp::config::asio> server;
    // pull out the type of messages sent by our config
    typedef server::message_ptr message_ptr;

    // Define a callback to handle incoming messages
    void on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
        std::cout << "on_message called with hdl: " << hdl.lock().get()
                << " and message: " << msg->get_payload()
                << std::endl;

        // check for a special command to instruct the server to stop listening so
        // it can be cleanly exited.
        if (msg->get_payload() == "stop-listening") {
            echo_server.stop_listening();
            return;
        }

        try {
            echo_server.send(hdl, msg->get_payload(), msg->get_opcode());
        } catch (websocketpp::exception const & e) {
            std::cout << "Echo failed because: "
                    << "(" << e.what() << ")" << std::endl;
        }
    }
    void on_open(websocketpp::connection_hdl hdl) {
        lock.lock();
        m_connections.insert(hdl);
        server::connection_ptr con = echo_server.get_con_from_hdl(hdl);
        std::cout << con->get_resource() << std::endl;
        lock.unlock();
    }

    void on_close(websocketpp::connection_hdl hdl) {
        lock.lock();
        m_connections.erase(hdl);
        lock.unlock();
    }

    void magic(int _port){
         try {
            // Set logging settings
            // echo_server.set_access_channels(websocketpp::log::alevel::all);
            // echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
            echo_server.set_access_channels(websocketpp::log::alevel::none);

            // Initialize Asio
            echo_server.init_asio();

            // Register our message handler
            std::function<void(websocketpp::connection_hdl, message_ptr)> cb = std::bind(&ServerTest::on_message, this, 
                                                                                            std::placeholders::_1, 
                                                                                            std::placeholders::_2);
            echo_server.set_message_handler(cb);


            echo_server.set_open_handler(std::bind(&ServerTest::on_open, this,_1));
            echo_server.set_close_handler(std::bind(&ServerTest::on_close, this,_1));

            // Listen on port 9002
            echo_server.listen(_port);

            // Start the server accept loop
            echo_server.start_accept();

            // Start the ASIO io_service run loop
            std::cout << "ready to listen in port " << _port << std::endl;
            echo_server.run();
        } catch (websocketpp::exception const & e) {
            std::cout << e.what() << std::endl;
        } catch (...) {
            std::cout << "other exception" << std::endl;
        }
    }

private:
    server echo_server; 
    
    typedef std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl>> con_list;
    con_list m_connections;

    std::mutex lock;
};



int main() {
    // Create a server endpoint
    
    ServerTest *s1 = nullptr;
    std::thread t1([&](){
        s1 = new ServerTest;
        s1->init(9002)  ;
    });
        std::this_thread::sleep_for(std::chrono::seconds(1));
    std::thread t2([&](){
        ServerTest s;
        s.init(9002);  
    });
    // std::thread t3([&](){
    //     ServerTest s;
    //     s.init(9002);   
    // });


    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(s1){
            s1->send("holaaaa");
        }
    }
}
