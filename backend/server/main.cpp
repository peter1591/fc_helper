#include <iostream>

#include <grpcpp/grpcpp.h>

#include "proto/service.grpc.pb.h"
#include "proto/service.pb.h"

using namespace grpc;

class ServiceImpl : public AIService::Service {
  Status run(ServerContext *, const RunRequest *req,
			ServerWriter< RunResponse> *resWriter) override {
    std::cout << "run() called with req:\n" << req->DebugString() << std::endl;

		RunResponse res;
    res.set_iteration(42);
		resWriter->Write(res);

		sleep(1);
    res.set_iteration(43);
		resWriter->Write(res);

		sleep(5);
    res.set_iteration(44);
		resWriter->Write(res);

    return Status::OK;
  }
};

int main(void) {
  ServiceImpl impl;

  ServerBuilder builder;
  builder.AddListeningPort("127.0.0.1:5783", InsecureServerCredentials());
  builder.RegisterService(&impl);
  std::unique_ptr<Server> server = builder.BuildAndStart();

  std::cout << "Starting server." << std::endl;
  server->Wait();

	std::cout << "Server stopped." << std::endl;
  return 0;
}
