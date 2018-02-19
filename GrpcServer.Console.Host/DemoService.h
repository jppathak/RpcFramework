#include <grpc++/impl/codegen/status.h>
#include "../DataTransferObjects/DataTransferApi.h"
#include "../GrpcContracts/GrpcClientServerContracts.h"

namespace DemoApplication
{
	class DemoService : public grpc::Service
	{
	public:
		DemoService();
		virtual ~DemoService();
		grpc::Status ProcessRequest(::grpc::ServerContext* context, const ::DataTransferApi::IMessage* request, ::DataTransferApi::IMessage* response);
		grpc::Status ProcessRequestServerStreaming(::grpc::ServerContext* context, const ::DataTransferApi::IMessage* request, ::grpc::ServerWriter<::DataTransferApi::IMessage>* writer);
	};
}
#pragma once
