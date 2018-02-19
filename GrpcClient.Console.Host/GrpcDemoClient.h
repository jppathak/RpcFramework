
#include <grpc++/impl/codegen/sync_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include "../DataTransferObjects/DataTransferApi.h"
#include "../GrpcContracts/GrpcClientServerContracts.h"
using namespace grpc;
using namespace DataTransferApi;
class GrpcDemoClient
{
public:
	GrpcDemoClient(const std::shared_ptr< ChannelInterface>& channel)
		:channel_(channel), 
		rpcmethod1_(ServiceContracts::methods[0]->Name(), ServiceContracts::methods[0]->MethodType(), channel),
		rpcmethod2_(ServiceContracts::methods[1]->Name(), ServiceContracts::methods[1]->MethodType(), channel)
	{}
	static std::unique_ptr<GrpcDemoClient> NewStub(const std::shared_ptr< ChannelInterface>& channel)
	{
		std::unique_ptr<GrpcDemoClient> stub(new GrpcDemoClient(channel));
		return stub;
	}
	
	Status SendRequest(ClientContext* context, const IMessage& request, IMessage* response)
	{
		return BlockingUnaryCall<IMessage, IMessage>(channel_.get(), rpcmethod1_, context, request, response);
	}

	ClientReader<IMessage>* SendRequestServerStreaming(ClientContext* context, const IMessage request)
	{
		return new ClientReader<IMessage>(channel_.get(), rpcmethod2_, context, request);
	}

//	ClientAsyncResponseReader<IMessage>* AsyncSendRequest(ClientContext* context, const IMessage request, CompletionQueue* cq)
//	{
//		return new ClientAsyncResponseReader<IMessage>(channel_.get(), cq, rpcmethod2_, context, request);
//	}

private:
	std::shared_ptr< ChannelInterface> channel_;
	const RpcMethod rpcmethod1_;
	const RpcMethod rpcmethod2_;

};
#pragma once
