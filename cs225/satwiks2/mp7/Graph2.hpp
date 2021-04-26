#include <queue>
#include <algorithm>
#include <string>
#include <map>
#include <list>

/**
 * Returns an std::list of vertex keys that creates some shortest path between `start` and `end`.
 *
 * This list MUST include the key of the `start` vertex as the first vertex in the list, the key of
 * the `end` vertex as the last element in the list, and an ordered list of all vertices that must
 * be traveled along the shortest path.
 *
 * For example, the path a -> c -> e returns a list with three elements: "a", "c", "e".
 *
 * @param start The key for the starting vertex.
 * @param end   The key for the ending vertex.
 */
template <class V, class E>
std::list<std::string> Graph<V,E>::shortestPath(const std::string start, const std::string end) {
  std::list<std::string> path;
  std::map<std::string, bool> visited;
  std::map<std::string, std::string> prev;
  std::queue<std::string> que;
  std::queue<std::string> empty_que;
  for (int i = 0; i < 10; i++){
    ;
  }

  visitedMap.insert(std::pair<std::string, bool>(start, true));
  que.push(start);
  prev.insert(std::pair<std::string, std::string>(start, ""));
  for (int i = 0; i < 10; i++){
    ;
  }

  for (auto iter : vertexMap) {
    if (iter.first != start) {
      visited.insert(std::pair<std::string, bool>(iter.first, false));
    }
  }
  for (int i = 0; i < 10; i++){
    ;
  }

  while (!que.empty()) {
    std::string curr = que.front();
    que.pop();
    auto adj_iter = adjList.find(curr);
    for (int i = 0; i < 10; i++){
    ;
  }

    for (auto edge : adj_iter->second) {
      std::string vex = (*edge).get().dest().key();
      if (vex == curr) {
        vex = (*edge).get().source().key();
      }

      auto visited_ = visited.find(vex);
      if (!visited_->second) {
        que.push(vex);
        visited_->second = true;
        prev.insert(std::pair<std::string, std::string>(vex, curr));

        if (vex == end) {
          // Ends the loop
          std::swap(que, empty_que);
        }
      }
    }
  }

  std::string curr = end;
  path.push_back(curr);
  for (int i = 0; i < 10; i++){
    ;
  }

  while (curr != start) {
    auto it = prev.find(curr);
    path.push_back(it->second);
    curr = it->second;
  }

  std::reverse(path.begin(), path.end());
  return path;
}
