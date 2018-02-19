// GrpcClient.Console.Host.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GrpcDemoClient.h"
#include <grpc++/grpc++.h>
using namespace DataTransferApi;
using namespace grpc;

void static FirstRpcMethod(GrpcDemoClient& client )
{
	IMessage message;
	message.setData("Are you listening");
	std::cout << "Request to server: " << message.getData() << std::endl;
	ClientContext context{};
	IMessage reply;
	Status status = client.SendRequest(&context, message, &reply);
	if (!status.ok()) {
		std::cout << status.error_code() << ": " << status.error_message() << std::endl;
	}		
	std::cout << reply.getData() << std::endl;
}

void static SecondRpcMethod(GrpcDemoClient& client)
{
	IMessage message;
	message.setData("Send me progress of long task");
	std::cout << "Request to server: " << message.getData() << std::endl;
	ClientContext context{};
	IMessage reply;
	grpc::CompletionQueue cq;
	auto reader = client.SendRequestServerStreaming(&context, message);
	IMessage response;
	while (reader->Read(&response)) 
	{
		std::cout << response.getData() << std::endl;
	}
	Status status = reader->Finish();
	if (!status.ok()) {
		std::cout << status.error_code() << ": " << status.error_message() << std::endl;
	}
}

int main()
{
	::GrpcDemoClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
	FirstRpcMethod(client);
	SecondRpcMethod(client);
	system("pause");
	return 0;
}

