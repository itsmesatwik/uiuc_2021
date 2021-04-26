#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"

#include <string>
#include <iostream>

/**
* @return The number of vertices in the Graph
*/
template <class V, class E>
unsigned int Graph<V,E>::size() const {
  return vertexMap.size();
}


/**
* @return Returns the degree of a given vertex.
* @param v Given vertex to return degree.
*/
template <class V, class E>
unsigned int Graph<V,E>::degree(const V & v) const {
  auto it = adjList.find(v.key());

  return it->second.size();
}

/**
* Inserts a Vertex into the Graph by adding it to the Vertex map and adjacency list
* @param key The key of the Vertex to insert
* @return The inserted Vertex
*/
template <class V, class E>
V & Graph<V,E>::insertVertex(std::string key) {
  V &v = *(new V(key));
  std::list<edgeListIter> list;

  vertexMap.insert(std::pair<std::string, V &>(key, v));
  adjList[key] = list;

  return v;
}


/**
* Removes a given Vertex
* @param v The Vertex to remove
*/
template <class V, class E>
void Graph<V,E>::removeVertex(const std::string & key) {
  vertexMap.erase(key);
  adjList.erase(key);
}


/**
* Inserts an Edge into the adjacency list
* @param v1 The source Vertex
* @param v2 The destination Vertex
* @return The inserted Edge
*/
template <class V, class E>
E & Graph<V,E>::insertEdge(const V & v1, const V & v2) {
  E & e = *(new E(v1, v2));
  auto iter1 = adjList.find(v1.key());
  auto iter2 = adjList.find(v2.key());

  edgeList.push_front(e);
  auto edgeIter = edgeList.begin();

  iter1->second.push_front(edgeIter);
  iter2->second.push_front(edgeIter);

  return e;
}


/**
* Removes an Edge from the Graph
* @param key1 The key of the ource Vertex
* @param key2 The key of the destination Vertex
*/
template <class V, class E>
void Graph<V,E>::removeEdge(const std::string key1, const std::string key2) {
  std::list<edgeListIter> edg = adjList.find(key1)->second;
  edgeListIter removeEdge;
  bool flag = false;

  for (auto edge : edg) {
    if ((*edge).get().dest().key() == key2) {
      removeEdge = edge;
      flag = !flag ? !flag : flag;
    }
  }
  if (flag) {
    edgeList.erase(removeEdge);
    edg.remove(removeEdge);
    std::list<edgeListIter> edg2 = (*(adjList.find(key2))).second;
    edg2.remove(removeEdge);
  }

}


/**
* Removes an Edge from the adjacency list at the location of the given iterator
* @param it An iterator at the location of the Edge that
* you would like to remove
*/
template <class V, class E>
void Graph<V,E>::removeEdge(const edgeListIter & it) {
  for (auto &iter1 : adjList) {
    for (auto &iter2 : iter1.second) {
      if (iter2 == it) {
        iter1.second.remove(it);
      }
    }
  }
}


/**
* @param key The key of an arbitrary Vertex "v"
* @return The list edges (by reference) that are adjacent to "v"
*/
template <class V, class E>
const std::list<std::reference_wrapper<E>> Graph<V,E>::incidentEdges(const std::string key) const {
  std::list<std::reference_wrapper<E>> edg;
  std::list<edgeListIter> edgelist = adjList.find(key)->second;

  for (auto item : edgelist) {
    edg.push_back(*item);
  }
  return edg;
}


/**
* Return whether the two vertices are adjacent to one another
* @param key1 The key of the source Vertex
* @param key2 The key of the destination Vertex
* @return True if v1 is adjacent to v2, False otherwise
*/
template <class V, class E>
bool Graph<V,E>::isAdjacent(const std::string key1, const std::string key2) const {
  std::list<edgeListIter> edg = adjList.find(key1)->second;
  bool flag = false;
  for (auto item : edg) {
    if ((*item).get().dest().key() == key2) {
      flag = true;
    }
  }
  return flag;
}
