#ifndef DGRAPH_H
#define DGRAPH_H

#include <grpc++/grpc++.h>
#include "dgraphclient.pb.h"
#include "dgraphclient.grpc.pb.h"

#include "clientstub.h"
#include <vector>

class Client
{
    public:
    Client();

    private:
    std::vector<DgraphClientStub>

};

#endif // DGRAPH_H