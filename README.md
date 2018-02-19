# RpcFramework
An Rpc framework in C++ customized from Grpc without any traces of automatic stub generation or protobuf

Goals:
The goal of this code is to use Grpc as pure rpc framework, keeping things clean with no auto-generated stubs by protoc compilation.
The advantage of this approach allows plugging grpc into already existing code where perhaps you already may have defined your own
data model or data transfer layers and custom serilizations.

Another goal is to keep data layers and serilizations completety swapable to number of choices that we might have including: 
1) Protobuf
2) Json/Xml
3) Datacontract like in .Net

Key Details:
1)  Fortunately, Grpc C++ implementations allows you to provide your own implmentations of SerializationTraits so long as they follow 
    the specs below for input and output. The key is to be able to serialize your data into grpc_byte_buffer* and back. 
 
 namespace grpc {
      template <class T>
      class SerializationTraits<T, typename std::enable_if<std::is_base_of<::DataTransferApi::IMessage, T>::value>::type>
      {
      public:
        static grpc::Status Serialize(const ::DataTransferApi::IMessage& msg, grpc_byte_buffer** bp, bool* own_buffer);
        static grpc::Status Deserialize(grpc_byte_buffer* buffer, ::DataTransferApi::IMessage* msg, int max_message_size);
      };
 
 2) The next step was to define a contract between client and server, which is defined as below:
   
 namespace ServiceContracts {
      class Method {
      public:
        Method(const char* name, const RpcMethod::RpcType type): methodName_(name), method_type_(type) {}
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

3) Using the contracts define a service class derived from grpc::Service as I below. I used console hosting of service but in production code we use grpc as a C++ service. 
  	
 DemoService::DemoService() {
      Service::AddMethod(new RpcServiceMethod(ServiceContracts::methods[0]->Name(),
        ServiceContracts::methods[0]->MethodType(),new RpcMethodHandler<DemoService, IMessage, IMessage>
        (std::mem_fn(&DemoApplication::DemoService::ProcessRequest), this)));

      Service::AddMethod(new RpcServiceMethod(ServiceContracts::methods[1]->Name(),
        ServiceContracts::methods[1]->MethodType(), new ServerStreamingHandler<DemoService, IMessage, IMessage>
        (std::mem_fn(&DemoApplication::DemoService::ProcessRequestServerStreaming), this)));
    }

4) Implement the service methods on server side.
5) Implement the client side minimum code needed making use of contracts defined:  
    
    GrpcDemoClient(const std::shared_ptr< ChannelInterface>& channel)
      :channel_(channel), 
      rpcmethod1_(ServiceContracts::methods[0]->Name(), ServiceContracts::methods[0]->MethodType(), channel),
      rpcmethod2_(ServiceContracts::methods[1]->Name(), ServiceContracts::methods[1]->MethodType(), channel){}
    static std::unique_ptr<GrpcDemoClient> NewStub(const std::shared_ptr< ChannelInterface>& channel)
    {
      std::unique_ptr<GrpcDemoClient> stub(new GrpcDemoClient(channel));
      return stub;
    }
    ...
That's it.
    
Building:
    I used Visual studio 2015 and downloaded Grpc.cpp and its dependencies through Nuget. I realized we need zlib and openssl as             additional dependencies for console hosing on client and server side. There is no need to bring in Protobuf or any custom               compilation.The full list is in packages folder. The sln file is also checked in with all projects. 
   
    
  

    
