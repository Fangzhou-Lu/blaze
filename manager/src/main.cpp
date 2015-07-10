#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Comm.h"
#include "QueueManager.h"
#include "BlockManager.h"
#include "TaskManager.h"

using namespace acc_runtime;

int main(int argc, char** argv) {
  
  int port = 1027;
  std::string ip_address = "127.0.0.1";

  if (argc > 1) {
    ip_address.assign(argv[1]);
  }
  if (argc > 2) {
    port = atoi(argv[2]);   
  }

  Logger logger(3);
  BlockManager block_manager(&logger);
  QueueManager queue_manager(&logger);

  // add task queue to queue manager
  queue_manager.add(
    "SimpleAddition", 
    "../../task/lib/SimpleAddition.so");

  TaskManager_ptr task_manager = 
        queue_manager.get("SimpleAddition");

  // start executor and commitor
  boost::thread executor(
    boost::bind(&TaskManager::execute, task_manager));

  boost::thread committer(
    boost::bind(&TaskManager::commit, task_manager));

  Comm comm(
          &block_manager, 
          &queue_manager, 
          &logger, ip_address, port);

  comm.listen();

  return 0;
}
