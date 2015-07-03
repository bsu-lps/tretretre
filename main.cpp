#include "run.h"

#include <mongo/client/dbclient.h>

#include <iostream>
#include <memory>

using namespace std;
using namespace mongo;

int main()
{
    client::GlobalInstance instance;
    if (!instance.initialized()) {
        std::cout << "failed to initialize the client driver: " << instance.status() << std::endl;
        return EXIT_FAILURE;
    }

    const string uri = "mongodb://localhost:27017";
    string errmsg;

    ConnectionString cs = ConnectionString::parse(uri, errmsg);

    if (!cs.isValid()) {
        cout << "Error parsing connection string " << uri << ": " << errmsg << endl;
        return EXIT_FAILURE;
    }

    unique_ptr<DBClientBase> conn(cs.connect(errmsg));
    if ( !conn ) {
        cout << "couldn't connect : " << errmsg << endl;
        return EXIT_FAILURE;
    }

    int ret = EXIT_SUCCESS;
    try {
        ret = run(conn.get());
    }
    catch( exception &e ) {
        cout << "caught " << e.what() << endl;
        ret = EXIT_FAILURE;
    }
    return ret;
}
