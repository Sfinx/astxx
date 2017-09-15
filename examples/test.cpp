
#include <astxx/manager.h>
#include <string>
#include <iostream>

namespace manager = astxx::manager;
namespace action = astxx::manager::action;
manager::connection *connection;
using namespace std;

void process_event(astxx::manager::message::event e)
{
 cerr << "got event: " << e["Event"] << endl;
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