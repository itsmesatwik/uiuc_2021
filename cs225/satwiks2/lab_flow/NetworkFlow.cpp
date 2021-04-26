/**
 * @file NetworkFlow.cpp
 * CS 225: Data Structures
 */

#include <vector>
#include <algorithm>
#include <set>

#include "graph.h"
#include "edge.h"

#include "NetworkFlow.h"

int min(int a, int b) {
  if (a<b)
    return a;
  else return b;
}

NetworkFlow::NetworkFlow(Graph & startingGraph, Vertex source, Vertex sink) :
  g_(startingGraph), residual_(Graph(true,true)), flow_(Graph(true,true)), source_(source), sink_(sink) {

  // YOUR CODE HERE
  std::vector<Vertex> v1;
  std::vector<Vertex> v2;
  std::vector<Vertex> vertex_paths = g_.getVertices();
  if(vertex_paths.size() != 0){
      for(size_t i = 0; i < vertex_paths.size(); i++){
        residual_.insertVertex(vertex_paths[i]);
      }
      for(size_t i = 0; i < vertex_paths.size(); i++){
        flow_.insertVertex(vertex_paths[i]);
      }
      for(size_t i = 0; i < vertex_paths.size(); i++){
        Vertex insert_vertex = vertex_paths[i];
        for(size_t j = 0; j < vertex_paths.size(); j++){
          if(g_.edgeExists(insert_vertex, vertex_paths[j])){
            v1.push_back(insert_vertex);
            v2.push_back(vertex_paths[j]);
            residual_.insertEdge(insert_vertex, vertex_paths[j]);
            residual_.setEdgeWeight(insert_vertex, vertex_paths[j], g_.getEdgeWeight(insert_vertex, vertex_paths[j]));
            flow_.insertEdge(insert_vertex, vertex_paths[j]);
            flow_.setEdgeWeight(insert_vertex, vertex_paths[j], 0);
          }
        }
      }
      for(size_t i = 0; i < v1.size(); i++){
        residual_.insertEdge(v2[i],v1[i]); 
      }
      for(size_t i = 0; i < v1.size(); i++){
        residual_.setEdgeWeight(v2[i],v1[i],0);
      }
      maxFlow_ = 0;
    }
  }


  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the helper function.
   *
   * @@params: source -- The starting (current) vertex
   * @@params: sink   -- The destination vertex
   * @@params: path   -- The vertices in the path
   * @@params: visited -- A set of vertices we have visited
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink,
  std::vector<Vertex> &path, std::set<Vertex> &visited) {

  if (visited.count(source) != 0)
    return false;
  
  visited.insert(source);

  if (source == sink) {
    return true;
  }

  vector<Vertex> adjs = residual_.getAdjacent(source);
  for(auto it = adjs.begin(); it != adjs.end(); it++) {
    if (visited.count(*it) == 0 && residual_.getEdgeWeight(source,*it) > 0) {
      path.push_back(*it);
      if (findAugmentingPath(*it,sink,path,visited))
        return true;
      else {
        path.pop_back();
      }
    }
  }

  return false;
}

  /**
   * findAugmentingPath - use DFS to find a path in the residual graph with leftover capacity.
   *  This version is the main function.  It initializes a set to keep track of visited vertices.
   *
   * @@params: source -- The starting (current) vertex
   * @@params: sink   -- The destination vertex
   * @@params: path   -- The vertices in the path
   */

bool NetworkFlow::findAugmentingPath(Vertex source, Vertex sink, std::vector<Vertex> &path) {
   std::set<Vertex> visited;
   path.clear();
   path.push_back(source);
   return findAugmentingPath(source,sink,path,visited);
}

  /**
   * pathCapacity - Determine the capacity of a path in the residual graph.
   *
   * @@params: path   -- The vertices in the path
   */

int NetworkFlow::pathCapacity(const std::vector<Vertex> & path) const {
  // YOUR CODE HERE
  if(path.size() >= 2){
    int edge_capacity = residual_.getEdgeWeight(path[1], path[0]);
    for(size_t i = 1; i < path.size()-1; i++){
      int minimum = residual_.getEdgeWeight(path[i], path[i+1]);
      if(minimum <= edge_capacity){
        edge_capacity = minimum;
      }
    }
    return edge_capacity;
  }   
  return 0;
}

  /**
   * calculuateFlow - Determine the capacity of a path in the residual graph.
   * Sets the member function maxFlow_ to be the flow, and updates the
   * residual graph and flow graph according to the algorithm.
   *
   * @@outputs: The network flow graph.
   */

const Graph & NetworkFlow::calculateFlow() {
  // YOUR CODE HERE
  std::vector<Vertex> vertex_list = g_.getVertices();
  std::vector<Vertex> vertex_paths;
  while(findAugmentingPath(source_,sink_, vertex_paths)){
    int cap = pathCapacity(vertex_paths);
    maxFlow_ += cap;
    for(size_t i = 0; i < vertex_paths.size() - 1; i++){
      if(flow_.edgeExists(vertex_paths[i], vertex_paths[i+1])){
        int cap_flow = flow_.getEdgeWeight(vertex_paths[i], vertex_paths[i+1]);
        flow_.setEdgeWeight(vertex_paths[i], vertex_paths[i+1], cap_flow+cap);
      }
    }
    for(size_t i = 0; i < vertex_paths.size() - 1; i++){
      int cap_r = residual_.getEdgeWeight(vertex_paths[i], vertex_paths[i+1]);
      residual_.setEdgeWeight(vertex_paths[i], vertex_paths[i+1], cap_r-cap);
      residual_.setEdgeWeight(vertex_paths[i+1], vertex_paths[i], cap_r+cap);
    }
  }
  return flow_;
}

int NetworkFlow::getMaxFlow() const {
  return maxFlow_;
}

const Graph & NetworkFlow::getGraph() const {
  return g_;
}

const Graph & NetworkFlow::getFlowGraph() const {
  return flow_;
}

const Graph & NetworkFlow::getResidualGraph() const {
  return residual_;
}