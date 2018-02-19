#include "stdafx.h"
#include "DemoService.h"
#include <sstream>
#include <grpc++/impl/codegen/method_handler_impl.h>
using namespace grpc;
using namespace DataTransferApi;

namespace DemoApplication {
	DemoService::DemoService()
	{
		Service::AddMethod(new RpcServiceMethod(ServiceContracts::methods[0]->Name(),
			ServiceContracts::methods[0]->MethodType(),new RpcMethodHandler<DemoService, IMessage, IMessage>
			(std::mem_fn(&DemoApplication::DemoService::ProcessRequest), this)));

		Service::AddMethod(new RpcServiceMethod(ServiceContracts::methods[1]->Name(),
			ServiceContracts::methods[1]->MethodType(), new ServerStreamingHandler<DemoService, IMessage, IMessage>
			(std::mem_fn(&DemoApplication::DemoService::ProcessRequestServerStreaming), this)));
	}

	DemoService::~DemoService()
	{
	}

	Status DemoService::ProcessRequest(ServerContext* context, const IMessage* request, IMessage* response)
	{
		std::ostringstream sStream;
		sStream << "Yes, I'm";
		response->setData(sStream.str());
		return ::grpc::Status(::grpc::StatusCode::OK, "Done processing request");
	}
	// simulating a long task
	Status DemoService::ProcessRequestServerStreaming(ServerContext* context, const IMessage* request, ServerWriter<IMessage>* writer)
	{
		std::ostringstream sStream;
		sStream << "Progress..";
		IMessage message;
		message.setData(sStream.str());
		writer->Write(message);

		for (int i = 0; i <= 100; i += 5)
		{
			std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(200));
			IMessage message;
			std::ostringstream sStream1;
			sStream1 << i,"% of", 100, "%";
			message.setData(sStream1.str());
			writer->Write(message);
		}
		IMessage message1;
		message1.setData("Server response completed");
		writer->Write(message1);
		return ::grpc::Status(::grpc::StatusCode::OK, "Done processing request server streaming");
	}
}