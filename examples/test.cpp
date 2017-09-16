
#include <astxx/manager.h>
#include <string>
#include <iostream>
#include <thread>

namespace manager = astxx::manager;
namespace action = astxx::manager::action;
namespace ph = std::placeholders;
using namespace std;

manager::connection *connection;

void process_event(astxx::manager::message::event e)
{
 cerr << "got event: " << e["Event"] << endl;
}

void show_channels()
{
 static int i = 30000;
 while(1) {
   action::command cmd("pjsip show channels");
   cmd.action_id(to_string(i++));
   auto done = [&] (manager::message::response r) -> void {
     cerr << "pjsip show channels done: \n" << r.format();
   };
   connection->send_action_async(cmd, done);
   usleep(100000);
 }
}

class done_t {
 public:
   void done(manager::message::response r) {
     cerr << "pjsip show endpoints done: \n" << r.format();
   }
};

void show_endpoints()
{
 done_t d;
 static int i = 10000;
 while(1) {
   action::command cmd("pjsip show endpoints");
   cmd.action_id(to_string(i++));
   connection->send_action_async(cmd, bind(&done_t::done, &d, ph::_1));
   usleep(110000);
 }
}

int main(int argc, char **argv)
{
 string ami_host = "localhost";
 string ami_name = "admin";
 string ami_pass = "mysecret";
 try {
   connection = new manager::connection(ami_host);
   cerr << "Connected to " << connection->name() << " v" <<
         connection->version() << endl;
   boost::signals2::scoped_connection c(connection->register_event("", process_event));
   action::login login(ami_name, ami_pass);
   login(*connection);
   thread thr1(show_channels);
   thr1.detach();
   thread thr2(show_endpoints);
   thr2.detach();
   while(1) {
     connection->wait_event();
     connection->pump_messages();
     connection->process_events();
   }
   return 0;
 } catch (const manager::action::channel_not_found &c) {
      cerr << "main: channel not found " << endl;
 } catch (const exception& e) {
      cerr << "main: exception: " << e.what() << endl;
 }
}