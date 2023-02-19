#include <iostream>

#include <grpcpp/grpcpp.h>

#include "ai/ai.h"
#include "proto/service.grpc.pb.h"
#include "proto/service.pb.h"
#include "proto_utils.h"

using namespace grpc;

using wiring::AIService;
using wiring::RunRequest;
using wiring::RunResponse;

class ServiceImpl : public AIService::Service {
  Status run(ServerContext *, const RunRequest *req,
             ServerWriter<RunResponse> *resWriter) override {
    std::cout << "run() called with req:\n" << req->DebugString() << std::endl;

    AI ai;
    ai.run(toAiRequest(*req), [&](const AIResult &aiResult) {
      resWriter->Write(fromAiResult(aiResult));
    });

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
