#ifndef DGRAPH_CLIENT_STUB_H
#define DGRAPH_CLIENT_STUB_H

#include <grpc++/grpc++.h>
#include "dgraphclient.pb.h"
#include "dgraphclient.grpc.pb.h"

#include "transaction.h"

class DgraphClientStub
{
    /*
    Stub for the Dgraph grpc client
    */
public:
    DgraphClientStub(const char *addr);

    grpc::Status Login();

    // Runs alter operation
    grpc::Status Alter();

    // Async version of alter
    //AsyncAlter();

    // Runs query or mutate operation
    grpc::Status Query();

    // Async version of query
    //AsyncQuery();

    // Runs commit or abort operation
    grpc::Status CommitOrAbort();

    // Returns the version of the Dgraph instance
    grpc::Status CheckVersion();

    // Deletes channel and stub
    void Close();
    
private:
    api::Dgraph::Stub stub;
    grpc::Channel channel;
};

#endif // DGRAPH_CLIENT_STUB_H