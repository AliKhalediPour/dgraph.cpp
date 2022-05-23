#ifndef DGRAPH_CLIENT_H
#define DGRAPH_CLIENT_H

#include <grpc++/grpc++.h>
#include "dgraphclient.pb.h"
#include "dgraphclient.grpc.pb.h"

#include <vector>
#include <string>
#include "clientstub.h"
#include "transaction.h"

class DgraphClient
{
    /*
       Creates a new Client for interacting with the Dgraph store.
       The client can be backed by multiple connections (to the same server, or
       multiple servers in a cluster).
   */

public:
    DgraphClient();

    // Returns the version of Dgraph if the server is ready to accept requests
    std::string& CheckVersion();

    void Login();

    void LoginIntoNamespace();

    void RetryLogin();

    // Runs a modification via this client
    void Alter();

    // The async version of alter
    //AsyncAlter();

    // Creates a transaction
    Transaction Txn();

    // Returns a random gRPC client so that requests are distributed evenly among them
    DgraphClientStub AnyClient();

    google::protobuf::Metadata AddLoginMetadata();

private:
    std::vector<DgraphClientStub> _clients;
    api::Jwt _jwt;
    std::vector<google::protobuf::Metadata> _login_metadata;
};

#endif // DGRAPH_CLIENT_H