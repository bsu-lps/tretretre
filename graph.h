#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <cassert>

namespace std
{
    template<>
    struct hash<std::pair<int,int> >
    {
        typedef std::pair<int,int>  argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type& e) const
        {
            long long v = e.first;
            v <<= 32;
            v |= e.second;
            return std::hash<long long>()(v);
        }
    };
}

class Graph
{
public:
    typedef std::pair<int,int> Edge;

    typedef std::unordered_set<Edge> EdgesCont;
    typedef std::unordered_set<int> AdjVertexCont;

    typedef EdgesCont::const_iterator EdgeIter;
    typedef AdjVertexCont::const_iterator AdjIter;

    void addEdge(int u, int v) {
        if (u>v)
            std::swap(u,v);

        auto res = m_edges.emplace(u,v);

        if (res.second) {
            m_adjList[u].insert(v);
            m_adjList[v].insert(u);
        }
    }

    int adjCount(int u) const {
        return m_adjList.find(u)->second.size();
    }

    /**
     * @brief isAdj
     * @param u vertex that has list
     * @param v vertex to search in list
     * @return is @arg v adjacent @arg u
     */
    bool isAdj(int u, int v) const {
        auto it = m_adjList.find(u);

        assert(it != m_adjList.cend());

        return it->second.find(v) != it->second.cend();
    }

    std::pair<EdgeIter, EdgeIter> edgesIters() const {
        return std::make_pair(m_edges.cbegin(), m_edges.cend());
    }

    std::pair<AdjIter, AdjIter> adjIters(int u) const {
        auto it = m_adjList.find(u);

        assert(it != m_adjList.cend());

        return std::make_pair(it->second.cbegin(), it->second.cend());
    }

private:
    std::unordered_set<Edge> m_edges;
    std::unordered_map<int, AdjVertexCont> m_adjList;
};

#endif // GRAPH_H
