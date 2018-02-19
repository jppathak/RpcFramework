#include <grpc/byte_buffer.h>
#include <grpc/byte_buffer_reader.h>
#include <grpc++/impl/codegen/serialization_traits.h>
#include <grpc++/impl/codegen/sync_stream.h>
#include "../DataTransferObjects/DataTransferApi.h"


namespace grpc
{
	template <class T>
	class SerializationTraits<T, typename std::enable_if<std::is_base_of<::DataTransferApi::IMessage, T>::value>::type>
	{
	public:
		static grpc::Status Serialize(const ::DataTransferApi::IMessage& msg, grpc_byte_buffer** bp, bool* own_buffer)
		{
			*own_buffer = true;
			std::string serializedData;
			Status status(::grpc::StatusCode::OK, "");
			if (!::DataTransferApi::Serialization<::DataTransferApi::IMessage>::Serialize(msg, serializedData))
				return Status(StatusCode::INTERNAL, "Failed during serialization");

			gpr_slice slice = gpr_slice_from_copied_string(serializedData.c_str());
			*bp = grpc_raw_byte_buffer_create(&slice, 1);
			gpr_slice_unref(slice);
			return Status(StatusCode::OK, "");
		}

		static grpc::Status Deserialize(grpc_byte_buffer* buffer, ::DataTransferApi::IMessage* msg, int max_message_size)
		{
			if (buffer == nullptr)
				return Status(StatusCode::INTERNAL, "No payload");

			grpc_byte_buffer_reader reader;
			if (!grpc_byte_buffer_reader_init(&reader, buffer)) {
				// grpc_byte_buffer_reader_init can fail if the data sent by the server could not be decompressed for any reason
				msg = NULL;
				return Status(StatusCode::INTERNAL, "No message");
			}
			std::string strBuffer;
			gpr_slice slice = grpc_byte_buffer_reader_readall(&reader);
			std::copy(GPR_SLICE_START_PTR(slice), GPR_SLICE_END_PTR(slice), std::back_inserter(strBuffer));
			gpr_slice_unref(slice);
			grpc_byte_buffer_reader_destroy(&reader);

			if (!::DataTransferApi::Serialization<::DataTransferApi::IMessage>::Deserialize(strBuffer, *msg))
				return Status(StatusCode::INTERNAL, "Failed during deserialization");

			Status status(::grpc::StatusCode::OK, "");
			return status;
		}
	};

}

namespace ServiceContracts
{
	using namespace grpc;
	using namespace DataTransferApi;

	class Method
	{
	public:
		Method(const char* name, const RpcMethod::RpcType type)
			: methodName_(name), method_type_(type) {}

		const char* Name() const { return methodName_; }
		RpcMethod::RpcType MethodType() const { return method_type_; }

	private:
		const char* methodName_;
		const RpcMethod::RpcType method_type_;
	};

	static Method* methods[] = {
		new Method("ProcessRequest", RpcMethod::NORMAL_RPC),
		new Method("ProcessRequestServerStreaming", RpcMethod::SERVER_STREAMING)
	};
}
#pragma once
