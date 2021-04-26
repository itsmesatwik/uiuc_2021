/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
    if (first[curDim] < second[curDim]) {
        return true;
    } else if (first[curDim] == second[curDim]) {
        return first < second;
    }
    return false;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
    double currDist = 0;
    double potentialDist = 0;

    for (unsigned i = 0; i < Dim; i++) {
        currDist += (currentBest[i] - target[i]) * (currentBest[i] - target[i]);
    }

    for (unsigned j = 0; j < Dim; j++) {
        potentialDist += (potential[j] - target[j]) * (potential[j] - target[j]);
    }

    if (potentialDist < currDist) {
        return true;
    } else if (potentialDist == currDist) {
        return potential < currentBest;
    }
    return false;
}

template<int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>> &points, int low, int high, int pivotIndex, int k) {
    Point<Dim> pivot_ = points[pivotIndex];
    swap(points[pivotIndex], points[high]);
    int pivotIndex_ = low;

    for (int j = low; j < high; j++) {
        if (smallerDimVal(points[j], pivot_, k)) {
            swap(points[pivotIndex_], points[j]);
            pivotIndex_++;
        }
    }

    swap(points[high], points[pivotIndex_]);
    return pivotIndex_;
}

template<int Dim>
Point<Dim>& KDTree<Dim>::selectKth(vector<Point<Dim>> &points, int low, int high, int pivotIndex, int k) {
    if (low >= high) {
        return points[low];
    }

    int median = (low + high) / 2;
    int pivotIndex_ = partition(points, low, high, median, k);

    if (pivotIndex == pivotIndex_) {
        return points[pivotIndex];
    } else if (pivotIndex < pivotIndex_) {
        return selectKth(points, low, pivotIndex_ - 1, pivotIndex, k);
    } else {
        return selectKth(points, pivotIndex_ + 1, high, pivotIndex, k);
    }
}

template<int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildTree(vector<Point<Dim>>& points, int low, int high, int k) {
    if (low > high) {
        return NULL;
    }

    int median = (low + high) / 2;
    Point<Dim> point = selectKth(points, low, high, median, k);
    KDTreeNode* node = new KDTreeNode(point);

    node->left = buildTree(points, low, median - 1, (k + 1) % Dim);
    node->right = buildTree(points, median + 1, high, (k + 1) % Dim);
    return node;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    vector<Point<Dim>> points = newPoints;
    root = buildTree(points, 0, points.size() - 1, 0);
}

template<int Dim>
void KDTree<Dim>::sort(vector<Point<Dim>>& points) {

}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */

  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    double radius = calculateRadius(root->point, query);
    Point<Dim> start;
    return findNearest(root, query, start, radius, 0);
}

template<int Dim>
Point<Dim> KDTree<Dim>::findNearest(KDTreeNode *node, const Point<Dim> query, Point<Dim>& currBest, double& currRadius, int k) const {
    if (node == NULL) {
        return currBest;
    }

    if (smallerDimVal(query, node->point, k)) {
        currBest = findNearest(node->left, query, currBest, currRadius, (k + 1) % Dim);
        if (shouldReplace(query, currBest, node->point)) {
            currBest = node->point;
            currRadius = calculateRadius(currBest, query);
        }
        if (calcualteOverlap(query, node->point, k) <= currRadius) {
            currBest = findNearest(node->right, query, currBest, currRadius, (k + 1) % Dim);
        }
    } else {
        currBest = findNearest(node->right, query, currBest, currRadius, (k + 1) % Dim);
        if (shouldReplace(query, currBest, node->point)) {
            currBest = node->point;
            currRadius = calculateRadius(currBest, query);
        }

        if (calcualteOverlap(query, node->point, k) <= currRadius) {
            currBest = findNearest(node->left, query, currBest, currRadius, (k + 1) % Dim);
        }
    }

    cout << currBest << endl;

    return currBest;
}

template<int Dim>
double KDTree<Dim>::calculateRadius(Point<Dim> currBest, const Point<Dim> target) const {
    double dist = 0;
    for (int i = 0; i < Dim; i++) {
        dist += ((target[i] - currBest[i]) * (target[i] - currBest[i]));
    }
    return sqrt(dist);
}

template<int Dim>
double KDTree<Dim>::calcualteOverlap(Point<Dim> target, Point<Dim> currPoint, int k) const {
    return sqrt((currPoint[k] - target[k]) * (currPoint[k] - target[k]));
}





// /**
//  * @file kdtree.cpp
//  * Implementation of KDTree class.
//  */
//
// #include <utility>
// #include <algorithm>
// #include <cmath>
//
// using namespace std;
//
// template <int Dim>
// bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
//                                 const Point<Dim>& second, int curDim) const
// {
//     /**
//      * @todo Implement this function!
//      */
//     //cout<<"Seg1"<<endl;
//     if (first[curDim] == second[curDim]) {
//       //cout<<"Seg2"<<endl;
//       return first < second;
//     }
//     else {
//       //cout<<"Seg3"<<endl;
//       return first[curDim] < second[curDim];
//     }
// }
//
// template <int Dim>
// bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
//                                 const Point<Dim>& currentBest,
//                                 const Point<Dim>& potential) const
// {
//     /**
//      * @todo Implement this function!
//      */
//     double dist1 = 0;
//     double dist2 = 0;
//
//     for (int i = 0; i < Dim; i++) {
//         dist1 += pow((target[i] - currentBest[i]), 2);
//         dist2 += pow((target[i] - potential[i]),2);
//     }
//
//     // dist1 = pow(dist1,0.5);
//     // dist2 = pow(dist2, 0.5);
//
//     if (dist1 == dist2)
//         return potential < currentBest;
//     else
//         return dist1 < dist2;
// }
//
// template <int Dim>
// KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
// {
//     // if (newPoints.size() == 0) {
//     //     root = nullptr;
//     //     size = 0;
//     // }
//     root = recurse_tree_build(newPoints, 0, newPoints.size()-1, 0);
//
//     /**
//      * @todo Implement this function!
//      */
// }
//
// template <int Dim>
// KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
//   /**
//    * @todo Implement this function!
//    */
//
// }
//
// template <int Dim>
// const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
//   /**
//    * @todo Implement this function!
//    */
//
//   return *this;
// }
//
// template <int Dim>
// KDTree<Dim>::~KDTree() {
//   /**
//    * @todo Implement this function!
//    */
//    clear(root);
//
// }
//
// template<int Dim>
// void KDTree<Dim>::clear(KDTreeNode* subroot) {
//   if (subroot == NULL) {return;}
//   if (subroot->left != NULL) {clear(subroot->left);}
//   if (subroot->right != NULL) {clear(subroot->right);}
//   delete subroot;
//   return;
//
// }
//
// template <int Dim>
// Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
// {
//     /**
//      * @todo Implement this function!
//      */
//      double radius = calculateRadius(root->point, query);
//      Point<Dim> start;
//      return findNearest(root, query, start, radius, 0);
//     return Point<Dim>();
// }
//
// template<int Dim>
// Point<Dim> KDTree<Dim>::findNearest(KDTreeNode *node, const Point<Dim> query, Point<Dim>& currBest, double& currRadius, int k) const {
//     if (node == NULL) {
//         return currBest;
//     }
//
//     if (smallerDimVal(query, node->point, k)) {
//         currBest = findNearest(node->left, query, currBest, currRadius, (k + 1) % Dim);
//         if (shouldReplace(query, currBest, node->point)) {
//             currBest = node->point;
//             currRadius = calculateRadius(currBest, query);
//         }
//         if (calcualteOverlap(query, node->point, k) <= currRadius) {
//             currBest = findNearest(node->right, query, currBest, currRadius, (k + 1) % Dim);
//         }
//     } else {
//         currBest = findNearest(node->right, query, currBest, currRadius, (k + 1) % Dim);
//         if (shouldReplace(query, currBest, node->point)) {
//             currBest = node->point;
//             currRadius = calculateRadius(currBest, query);
//         }
//
//         if (calcualteOverlap(query, node->point, k) <= currRadius) {
//             currBest = findNearest(node->left, query, currBest, currRadius, (k + 1) % Dim);
//         }
//     }
//
//     //cout << currBest << endl;
//
//     return currBest;
// }
//
// template<int Dim>
// double KDTree<Dim>::calculateRadius(Point<Dim> currBest, const Point<Dim> target) const {
//     double dist = 0;
//     for (int i = 0; i < Dim; i++) {
//         dist += ((target[i] - currBest[i]) * (target[i] - currBest[i]));
//     }
//     return sqrt(dist);
// }
//
// template<int Dim>
// double KDTree<Dim>::calcualteOverlap(Point<Dim> target, Point<Dim> currPoint, int k) const {
//     return sqrt((currPoint[k] - target[k]) * (currPoint[k] - target[k]));
// }
//
// template <int Dim>
// typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::recurse_tree_build(vector<Point<Dim>> points, int low, int high, int dim) {
//   if (low > high) {
//     return NULL;
//   }
//   Point<Dim> median = quick_select(points, low, high, dim);
//   int median_index = floor((low+high)/2);
//   KDTreeNode* subroot = new KDTreeNode(median);
//   subroot->left = recurse_tree_build(points, low, median_index - 1, ((dim+1)%Dim));
//   subroot->right = recurse_tree_build(points, median_index + 1, high, ((dim+1)%Dim));
//   return subroot;
// }
//
// template <int Dim>
// int KDTree<Dim>::partition(vector<Point<Dim>>& points, int low, int high, int dim) {
//   Point<Dim> pivot = points[high];
//   int iter = low;
//   for (int i  = low; i < high; i++) {
//     if (smallerDimVal(points[i], pivot, dim)) {
//       Point<Dim> temp = points[iter];
//       points[iter] = points[i];
//       points[i] = temp;
//       iter++;
//     }
//   }
//   Point<Dim> temp = points[iter];
//   points[iter] = points[high];
//   points[high] = temp;
//   return iter;
// }
//
// template <int Dim>
// Point<Dim> KDTree<Dim>::quick_select(vector<Point<Dim>>& points, int low, int high, int dim) {
//   if (low >= high) {
//     return points[low];
//   }
//   int median_index = floor((low+high)/2);
//   int pivotIndex = partition(points, low, high, dim);
//   if (median_index == pivotIndex) {
//     return points[median_index];
//   }
//   else if (median_index < pivotIndex) {
//     return quick_select(points, low, pivotIndex - 1, dim);
//   }
//   else {
//     return quick_select(points, pivotIndex + 1, high, dim);
//   }
// }
