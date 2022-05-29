#ifndef DGRAPH_TRANSACTION_H
#define DGRAPH_TRANSACTION_H

#include <grpc++/grpc++.h>
#include "dgraphclient.pb.h"
#include "dgraphclient.grpc.pb.h"

#include <memory>
#include "client.h"
#include "clientstub.h"
#include "exception"

class Txn
{
    /*
        Txn is a single atomic Txn.
        A Txn lifecycle is as follows:
        1. Created using Client.newTxn.
        2. Modified via calls to query and mutate.
        3. Committed or discarded. If any mutations have been made, it's important
        that at least one of these methods is called to clean up resources. Discard
        is a no-op if commit has already been called, so it's safe to call discard
        after calling commit.
    */

public:
    Txn(std::shared_ptr<DgraphClient> dgraphClient, bool read_only, bool best_effort);

    api::Response Query(std::string query, std::map<std::string, std::string> variables, uint64_t timeout, google::protobuf::Metadata metadata);
    // AsyncQuery();
    api::Response Mutate();
    // AsyncMutate();

    // Executes a query/mutate operation on the server
    api::Response DoRequest(api::Request request, uint64_t timeout);
    // AsyncDoRequest();

    api::Request CreateRequest(std::string query, std::map<std::string, std::string> variables, bool commitNow, api::Request_RespFormat respFormat);

    // Commits the Txn
    void Commit(uint64_t timeout, google::protobuf::Metadata *metadata);

    // Discard the Txn
    void Discard();

    // Merges context from this instance with src
    void MergeContext(api::TxnContext *src);

    void RetryLogin();

private:
    bool _read_only = false;
    bool _best_effort = false;
    bool _finished = false;
    bool _mutated = false;

    std::shared_ptr<DgraphClientStub> _dgraphClientStub;
    std::shared_ptr<DgraphClient> _dgraphClient;

    std::unique_ptr<api::TxnContext> _ctx;
};

Txn::Txn(std::shared_ptr<DgraphClient> dgraphClient, bool read_only, bool best_effort)
    : _dgraphClient(dgraphClient), _dgraphClientStub(_dgraphClient->AnyClient()), _read_only(read_only), _best_effort(best_effort)
{
    if (!read_only && best_effort)
        throw std::runtime_error("Best effort transactions are only compatible with read-only transactions");

    // self._dg = client => it means dgraphClient
    // self._dc = client.any_client() => it means dgraphClientStub

    this->_ctx = std::unique_ptr<api::TxnContext>(new api::TxnContext());
}

api::Response Txn::Query(std::string query, std::map<std::string, std::string> variables, uint64_t timeout, google::protobuf::Metadata metadata)
{
    auto req = this->CreateRequest(query, variables, false, api::Request_RespFormat::Request_RespFormat_RDF);
    return this->DoRequest(req, timeout);
}

api::Response Txn::Mutate()
{
}
api::Response Txn::DoRequest(api::Request request, uint64_t timeout)
{
    if (this->_finished)
    {
        throw std::runtime_error("Transaction has already been committed or discarded");
    }

    if (request.mutations().size())
    {
        if (this->_read_only)
            throw std::runtime_error("Readonly transaction cannot run mutations");

        this->_mutated = true;
    }

    request.set_hash(this->_ctx->hash());

    auto newMetadata = this->_dgraphClient->AddLoginMetadata(nullptr);
    api::Response response;
    this->_dgraphClientStub->Query(request, &response);

    return response;
}

api::Request Txn::CreateRequest(std::string query, std::map<std::string, std::string> variables, bool commitNow, api::Request_RespFormat respFormat)
{
    api::Request req;
    return req;
}
void Txn::Commit(uint64_t timeout, google::protobuf::Metadata *metadata)
{
    auto newMetadata = this->_dgraphClient->AddLoginMetadata(metadata);
    try
    {
    }
    catch (...)
    {
    }
}
void Txn::Discard()
{
}
void Txn::MergeContext(api::TxnContext *src)
{
    if (src == nullptr)
    {
        // This condition will be true only if the server doesn't return a
        // txn context after a query or mutation.
        return;
    }

    if (this->_ctx->start_ts() == 0)
    {
        this->_ctx->set_start_ts(src->start_ts());
    }
    else if (this->_ctx->start_ts() != src->start_ts())
    {
        // This condition should never be true.
        throw std::runtime_error("StartTs mismatch");
    }

    this->_ctx->set_hash(src->hash());

    // extend keys of source to _ctx
    auto keys = src->keys();
    for (int i = 0; i < keys.size(); i++)
    {
        this->_ctx->add_keys(keys.at(i));
    }
}

void Txn::RetryLogin()
{
    this->_dgraphClient->RetryLogin();
}

#endif // DGRAPH_TXN_H