#include "run.h"

#include "graph.h"

#include <iostream>
#include <fstream>
#include <limits>
#include <exception>
#include <tuple>
#include <unordered_set>

using namespace std;
using namespace mongo;

Graph* buildGraph(DBClientBase* conn)
{
    auto_ptr<DBClientCursor> cursor = conn->query("vkuu.friends_list", BSONObj());

    if (!cursor.get()) {
        cout << "query failure" << endl;
        return 0;
    }

    unique_ptr<Graph> g(new Graph);

    for (int i=0; cursor->more(); ++i ) {
        BSONObj r = cursor->next();

        int uid = r.getField("uid").numberInt();
        BSONObj friends = r.getObjectField("friends");

        BSONObjIterator frIter (friends);

        while (frIter.more()) {
            int fr = frIter.next().numberInt();
            g->addEdge(uid, fr);
        }

        if (i%5000==0)
            cout << "Vertex candidate " << i << " passed" << endl;
    }

    return g.release();
}

int run(DBClientBase* conn) {
    unique_ptr<Graph> g (buildGraph(conn));
    if (!g)
        return EXIT_FAILURE;

    cout << "Graph is builded." << endl;

    return EXIT_SUCCESS;

    long long treCnt = 0;

    int edgesCnt = 0;
    Graph::EdgeIter it, end;
    for (tie(it, end) = g->edgesIters(); it!=end; ++it) {
        ++edgesCnt;

        int u = it->first;
        int v = it->second;

        if (g->adjCount(u) < g->adjCount(v))
            swap(u,v);

        Graph::AdjIter frIt, frEnd;
        for (tie(frIt, frEnd) = g->adjIters(v); frIt != frEnd; ++frIt) {
            if (g->isAdj(u,v)) {
                ++treCnt;
                if (treCnt%10000 == 0)
                    cout << "Count is " << treCnt << "." << endl;
            }
        }

        if (edgesCnt%10000 == 0)
            cout << "Edges no " << edgesCnt << " has been processed." << endl;
    }

    cout << "Count is " << treCnt << "." << endl;
    return EXIT_SUCCESS;
}
