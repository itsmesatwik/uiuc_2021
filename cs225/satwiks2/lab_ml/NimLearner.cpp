/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "NimLearner.h"
#include <ctime>


/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true) {
    /* Your code goes here! */
    num_tokens_ = (int) startingTokens;
    for (int i = num_tokens_; i >= 0; i--) {
      Vertex p1 = "p" + to_string(1) + "-" + to_string(i);
      Vertex p2 = "p" + to_string(2) + "-" + to_string(i);
      g_.insertVertex(p1);
      g_.insertVertex(p2);

      if (i == num_tokens_) {
        startingVertex_ = p1;
        continue;
      }

      for (int j = 1; j <= 2; j++) {
        int prev = i + j;
        if (prev <= num_tokens_) {
          Vertex source1 = "p" + to_string(2) + "-" + to_string(prev);
          Vertex source2 = "p" + to_string(1) + "-" + to_string(prev);
          g_.insertEdge(source1, p1);
          g_.insertEdge(source2, p2);
          g_.setEdgeWeight(source1, p1, 0);
          g_.setEdgeWeight(source2, p2, 0);
        }
      }
    }
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const {
  vector<Edge> path;

  Vertex current = startingVertex_;
  int next = num_tokens_;
  bool cur_two = false;
  while (next > 0) {
    int player = (int)cur_two + 1;
    current = "p" + to_string(player) + "-" + to_string(next);

    if (next > 1) {
      int rand_dec = (rand() % 2) + 1;  //random number between 1 and 2
      next -= rand_dec;
    } else {
      next = 0;
    }

    cur_two = !cur_two;
    int player2 = (int)cur_two + 1;
    Vertex next_v = "p" + to_string(player2) + "-" + to_string(next);

    Edge e = g_.getEdge(current, next_v);
    path.push_back(e);

  }
  return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path) {
 /* Your code goes here! */
 Edge last_edge = path.back();
 Vertex last_v = last_edge.dest;

 int edge1_val, edge2_val;

 //player 1 wins
 if (last_v == "p2-0") {
   edge1_val = 1;
   edge2_val = -1;
 } else {
   edge2_val = 1;
   edge1_val = -1;
 }

 bool player1 = true;
 for (unsigned i = 0; i < path.size(); i++) {
   Edge cur = path.at(i);
   int weight = cur.getWeight();

   if (player1) {
     weight += edge1_val;
   } else {
     weight += edge2_val;
   }

   g_.setEdgeWeight(cur.source, cur.dest, weight);
   player1 = !player1;
 }
}

/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
  for (const Vertex & v : g_.getVertices()) {
    for (const Vertex & w : g_.getAdjacent(v)) {
      int weight = g_.getEdgeWeight(v, w);

      // Label all edges with positve weights as "WINPATH"
      if (weight > threshold)           { g_.setEdgeLabel(v, w, "WIN"); }
      else if (weight < -1 * threshold) { g_.setEdgeLabel(v, w, "LOSE"); }
    }
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}
