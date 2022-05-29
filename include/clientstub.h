#ifndef DGRAPH_CLIENT_STUB_H
#define DGRAPH_CLIENT_STUB_H

#include <grpc++/grpc++.h>
#include "dgraphclient.pb.h"
#include "dgraphclient.grpc.pb.h"

#include <stdio.h>
#include <memory>

#include "transaction.h"

class DgraphClientStub
{
    /*
    Stub for the Dgraph grpc client
    */
public:
    DgraphClientStub(const char *addr);

    grpc::Status Login(const api::LoginRequest &loginRequest, api::Response *response);

    // Runs alter operation
    grpc::Status Alter(const api::Operation &request, api::Payload *response);

    // Async version of alter
    // AsyncAlter();

    // Runs query or mutate operation
    grpc::Status Query(const api::Request request, api::Response *response);

    // Async version of query
    // AsyncQuery();

    // Runs commit or abort operation
    grpc::Status CommitOrAbort(const api::TxnContext &request, api::TxnContext *response, uint64_t timeout, google::protobuf::Metadata *metadata);

    // Returns the version of the Dgraph instance
    grpc::Status CheckVersion(const api::Check check, api::Version *response);

    // Deletes channel and stub
    void Close();

private:
    std::unique_ptr<api::Dgraph::Stub> stub;
    std::shared_ptr<grpc::ChannelInterface> channel;
};

DgraphClientStub::DgraphClientStub(const char *addr) : channel(grpc::CreateChannel(addr, grpc::InsecureChannelCredentials())), stub(api::Dgraph::NewStub(channel))
{
    // grpc::
}

grpc::Status DgraphClientStub::Login(const api::LoginRequest &loginRequest, api::Response *response)
{
    grpc::ClientContext context;
    return stub->Login(&context, loginRequest, response);
}

grpc::Status DgraphClientStub::Alter(const api::Operation &request, api::Payload *response)
{
    grpc::ClientContext context;
    return stub->Alter(&context, request, response);
}

grpc::Status DgraphClientStub::Query(const api::Request request, api::Response *response)
{
    grpc::ClientContext context;
    return stub->Query(&context, request, response);
}

grpc::Status DgraphClientStub::CommitOrAbort(const api::TxnContext &request, api::TxnContext *response, uint64_t timeout, google::protobuf::Metadata *metadata)
{
    grpc::ClientContext context;
    return this->stub->CommitOrAbort(&context, request, response);
}

grpc::Status DgraphClientStub::CheckVersion(const api::Check check, api::Version *response)
{
    grpc::ClientContext context;
    // ::grpc::ClientContext* context, const ::api::Check& request, ::api::Version* response
    return this->stub->CheckVersion(&context, check, response);
}

#endif // DGRAPH_CLIENT_STUB_H