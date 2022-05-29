#ifndef DGRAPH_UTIL_H
#define DGRAPH_UTIL_H

#include <grpc++/grpc++.h>
#include "dgraphclient.pb.h"
#include "dgraphclient.grpc.pb.h"

#include <stdlib.h>
#include <stdexcept>
#include <string.h>

namespace util
{

    bool isJwtExpired(void *error)
    {
        auto except = static_cast<std::exception *>(error);
        return strcmp(except->what(), "Token is expired") == 0;
    }

    // Returns true if the error is due to an aborted transaction
    bool isAbortedError(void *error)
    {
        return false;
    }

    bool isRetriableError(void *error)
    {
        return false;
    }

    bool isConnectionError(void *error)
    {
        return false;
    }
}

#endif // DGRAPH_UTIL_H