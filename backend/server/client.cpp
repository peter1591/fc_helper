#include <iostream>

#include <grpcpp/grpcpp.h>

#include "proto/service.grpc.pb.h"
#include "proto/service.pb.h"

using namespace grpc;

using wiring::AIService;
using wiring::RunRequest;
using wiring::RunResponse;

int main(void) {
	std::cout << "client\n";

	auto channel = CreateChannel("127.0.0.1:5783", InsecureChannelCredentials());
	auto stub = AIService::NewStub(channel);

	ClientContext context;
	RunRequest req;
	req.set_rand_seed(844);
	const auto reader = stub->run(&context, req);

	RunResponse res;
	while (reader->Read(&res)) {
		std::cout << "Got response:\n" << res.DebugString() << std::endl;
	}

	return 0;
}
