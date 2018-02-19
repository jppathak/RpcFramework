// GrpcServer.Console.Host.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DemoService.h"
#include <grpc++/grpc++.h>

void RunServer() {
	std::string server_address("0.0.0.0:50051");
	DemoApplication::DemoService serviceImpl;

	::grpc::ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address, ::grpc::InsecureServerCredentials());

	// Register "service" as the instance through which we'll communicate with
	// clients. 
	builder.RegisterService(&serviceImpl);

	// Finally assemble the server.
	std::unique_ptr<::grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;

	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
}


int main()
{
	RunServer();
    return 0;
}

