#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>

#include "task.pb.h"
#include "BlockManager.h"
#include "QueueManager.h"
#include "TaskManager.h"
#include "Logger.h"

using namespace boost::asio;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

/*
 * Communicator design for Acc_Manager
 *
 */

namespace acc_runtime {
class Comm {

public:
  Comm(
      BlockManager* _block_manager,
      QueueManager* _queue_manager,
      Logger* _logger,
      std::string address = "127.0.0.1",
      int ip_port = 1027
    ):
    ip_address(address), 
    srv_port(ip_port), 
    block_manager(_block_manager),
    queue_manager(_queue_manager),
    logger(_logger)
  { 
    ;
  }

  void recv(TaskMsg&, ip::tcp::iostream&);
  void send(TaskMsg&, ip::tcp::iostream&);
  void process(socket_ptr); // processing messages
  void listen(); // always on kernel to wait for connection
  
private:
  int srv_port;
  std::string ip_address;
  std::vector<boost::thread> thread_pool;

  // reference to block manager
  BlockManager *block_manager;
  // reference to queue manager
  QueueManager *queue_manager;

  Logger *logger;
};
}
