#include <fstream>
#include <iostream>

#include <grpcpp/grpcpp.h>

#include "ai/ai.h"
#include "proto/service.grpc.pb.h"
#include "proto/service.pb.h"
#include "proto_utils.h"

using namespace grpc;

using wiring::AIService;
using wiring::LoadRequest;
using wiring::LoadResponse;
using wiring::RunRequest;
using wiring::RunResponse;
using wiring::SaveRequest;
using wiring::SaveResponse;

class ServiceImpl : public AIService::Service {
  static constexpr char kDatabasePath[] = "/home/shenen/fchelper.state";

public:
  ServiceImpl() { loadDatabase(); }

private:
  void loadDatabase() {
    std::ifstream f(kDatabasePath, std::ios_base::in | std::ios_base::binary);
    database.ParseFromIstream(&f);
    std::cout << "loaded database:\n" << database.DebugString() << std::endl;
  }

  void saveDatabase() {
    std::cout << "saving database:\n" << database.DebugString() << std::endl;
    std::ofstream f(kDatabasePath, std::ios_base::out | std::ios_base::binary);
    database.SerializeToOstream(&f);
  }

  Status run(ServerContext *, const RunRequest *req,
             ServerWriter<RunResponse> *resWriter) override {
    std::cout << "run() called with req:\n" << req->DebugString() << std::endl;

    AI ai;
    ai.run(toAiRequest(*req), [&](const AIResult &aiResult) {
      resWriter->Write(fromAiResult(aiResult));
    });

    return Status::OK;
  }

  Status load(ServerContext *, const LoadRequest *request,
              LoadResponse *response) override {
    for (const auto &entry : database.saved_requests()) {
      if (request->name() != entry.name()) {
        continue;
      }
      *response->mutable_request() = entry.request();
      break;
    }
    return Status::OK;
  }

  Status save(ServerContext *, const SaveRequest *request,
              SaveResponse *) override {
		addToDatabase(*request);
		saveDatabase();
    return Status::OK;
  }

	void addToDatabase(const SaveRequest& request) {
    for (auto &entry : *database.mutable_saved_requests()) {
      if (request.name() != entry.name()) {
        continue;
      }
      *entry.mutable_request() = request.request();
			return;
    }

    auto *entry = database.mutable_saved_requests()->Add();
    entry->set_name(request.name());
    *entry->mutable_request() = request.request();
	}

  wiring::Database database;
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
