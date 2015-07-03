#ifndef RUN_H
#define RUN_H

#include <mongo/client/dbclient.h>

int run(mongo::DBClientBase* conn);

#endif // RUN_H
