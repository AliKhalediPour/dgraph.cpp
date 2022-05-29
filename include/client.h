#ifndef DGRAPH_CLIENT_H
#define DGRAPH_CLIENT_H

#include <grpc++/grpc++.h>
#include "dgraphclient.pb.h"
#include "dgraphclient.grpc.pb.h"

#include <vector>
#include <string>
#include "clientstub.h"
#include "transaction.h"
#include "util.h"

class DgraphClient
{
    /*
       Creates a new Client for interacting with the Dgraph store.
       The client can be backed by multiple connections (to the same server, or
       multiple servers in a cluster).
   */

public:
    DgraphClient(std::vector<std::shared_ptr<DgraphClientStub>> clients);

    // Returns the version of Dgraph if the server is ready to accept requests
    std::string CheckVersion(google::protobuf::Metadata *metadata);

    void Login();

    void LoginIntoNamespace();

    void RetryLogin();

    // Runs a modification via this client
    void Alter();

    // The async version of alter
    // AsyncAlter();

    // Creates a transaction
    Txn Transaction();

    // Returns a random gRPC client so that requests are distributed evenly among them
    std::shared_ptr<DgraphClientStub> AnyClient();

    std::vector<google::protobuf::Metadata> AddLoginMetadata(google::protobuf::Metadata *metadata);

private:
    std::vector<std::shared_ptr<DgraphClientStub>> _clients;
    api::Jwt _jwt;
    std::vector<google::protobuf::Metadata> _login_metadata;
};

DgraphClient::DgraphClient(std::vector<std::shared_ptr<DgraphClientStub>> clients) : _clients(clients), _jwt(api::Jwt())
{
    if (clients.size() == 0)
    {
        throw std::runtime_error("No clients provided in DgraphClient constructor");
    }
}

std::string DgraphClient::CheckVersion(google::protobuf::Metadata *metadata)
{
    auto newMetadata = this->AddLoginMetadata(metadata);
    auto check_req = api::Check();

    try
    {
        auto version = api::Version();
        auto response = this->AnyClient()->CheckVersion(check_req, &version);
        if (response.ok())
            return version.tag();
        else
            throw std::runtime_error("check version failed => response:");
    }
    catch (std::exception error)
    {
        if (util::isJwtExpired(&error))
        {
            this->RetryLogin();
            newMetadata = this->AddLoginMetadata(metadata);
            auto version = api::Version();
            auto response = this->AnyClient()->CheckVersion(check_req, &version);
            return version.tag();
        }
    }
}

std::vector<google::protobuf::Metadata> DgraphClient::AddLoginMetadata(google::protobuf::Metadata *metadata)
{
    if (metadata != nullptr)
        this->_login_metadata.push_back(*metadata);

    return this->_login_metadata;
}

#endif // DGRAPH_CLIENT_H