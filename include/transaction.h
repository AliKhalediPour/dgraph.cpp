#ifndef DGRAPH_TRANSACTION_H
#define DGRAPH_TRANSACTION_H

#include <grpc++/grpc++.h>
#include "dgraphclient.pb.h"
#include "dgraphclient.grpc.pb.h"

class Transaction
{
    /*
        Txn is a single atomic transaction.
        A transaction lifecycle is as follows:
        1. Created using Client.newTxn.
        2. Modified via calls to query and mutate.
        3. Committed or discarded. If any mutations have been made, it's important
        that at least one of these methods is called to clean up resources. Discard
        is a no-op if commit has already been called, so it's safe to call discard
        after calling commit.
    */

public:
    Transaction();

    api::Response Query();
    // AsyncQuery();
    api::Response Mutate();
    // AsyncMutate();
    api::Response DoRequest();
    // AsyncDoRequest();

    api::Request CreateRequest();

    // Commits the transaction
    void Commit();

    // Discard the transaction
    void Discard();

    // Merges context from this instance with src
    void MergeContext();

    void RetryLogin();
};

#endif // DGRAPH_TXN_H