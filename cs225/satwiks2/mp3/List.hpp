/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */


/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in MP3.1
  return List<T>::ListIterator(nullptr);
}

/**
 * Destroys the current List. This function should ensure that
 * memory does not leak on destruction of a list.
 */
template <typename T>
List<T>::~List() {
  this->_destroy();
  /// @todo Graded in MP3.1
}

/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in MP3.1
  if(head_ == NULL){
    return;
  }
  ListNode *iter = head_->next;
  while (iter->next) {
    delete iter->prev;
    iter = iter->next;
  }
  delete iter->prev;
  delete iter;
  head_ = nullptr;
  tail_ = nullptr;
  length_ = 0;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  ListNode *newNode = new ListNode(ndata);
  if (head_ == nullptr) {
    head_ = newNode;
    tail_ = newNode;
    length_++;
    return;
  }
  newNode->next = head_;
  head_->prev = newNode;
  head_ = newNode;
  length_++;

  /// @todo Graded in MP3.1
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in MP3.1
  ListNode *newNode = new ListNode(ndata);
  if (tail_ == nullptr) {
    head_ = newNode;
    tail_ = newNode;
    length_++;
    return;
  }
  tail_->next = newNode;
  newNode->prev = tail_;
  newNode->next = nullptr;
  tail_ = newNode;
  length_++;
}

/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in MP3.
  ListNode *newEnd = endPoint;
  bool first = true;
  while (startPoint != endPoint) {
    ListNode* temp = startPoint;
    startPoint = startPoint->next;
    if (temp->prev != nullptr) {
      (temp->prev)->next = startPoint;
    }
    startPoint->prev = (temp->prev);
    temp->next = endPoint->next;
    temp->prev = endPoint;
    endPoint->next = temp;
    if (temp->next != nullptr) {
      (temp->next)->prev = temp;
    }
    if (first) {
      newEnd = temp;
      first = false;
    }
  }
  endPoint = newEnd;
  if (startPoint->prev != nullptr) {
    (startPoint->prev)->next = startPoint;
  }
  if (endPoint->next != nullptr) {
    (endPoint->next)->prev = endPoint;
  }


}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in MP3.1
  if (n >= length_) {
    reverse(head_, tail_);
    return;
  }

  ListNode *temp1 = head_;
  ListNode *temp2 = head_;
  bool first = true;
  while (temp2 != nullptr) {
    int counter = 1;
    while(counter != n && temp2->next != nullptr) {
      temp2 = temp2->next;
      counter++;
    }
    if (first) {
      reverse(head_, temp2);
      first = false;
    }
    reverse(temp1, temp2);
    if (temp2->next == nullptr) {
      tail_ = temp2;
    }
    if (temp1 != nullptr) {
      temp1 = temp2->next;
      temp2 = temp2->next;
    }
  }


}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  bool remove_ = false;
  if (head_->next == nullptr) {
    return;
  }
  ListNode *iter = head_;

  while (iter->next != nullptr) {
    if (remove_) {
      ListNode *nextIter = iter->next;
      iter->prev->next = iter->next;
      iter->next->prev = iter->prev;
      iter->prev = tail_;
      tail_->next = iter;
      iter->next = nullptr;
      tail_ = iter;
      iter = nextIter;
      remove_ = false;
    }
    else {
      iter = iter->next;
      remove_ = true;
    }
  }
  /// @todo Graded in MP3.1
}

/**
 * Splits the given list into two parts by dividing it at the splitPoint.
 *
 * @param splitPoint Point at which the list should be split into two.
 * @return The second list created from the split.
 */
template <typename T>
List<T> List<T>::split(int splitPoint) {
    if (splitPoint > length_)
        return List<T>();

    if (splitPoint < 0)
        splitPoint = 0;

    ListNode * secondHead = split(head_, splitPoint);

    int oldLength = length_;
    if (secondHead == head_) {
        // current list is going to be empty
        head_ = NULL;
        tail_ = NULL;
        length_ = 0;
    } else {
        // set up current list
        tail_ = head_;
        while (tail_ -> next != NULL)
            tail_ = tail_->next;
        length_ = splitPoint;
    }

    // set up the returned list
    List<T> ret;
    ret.head_ = secondHead;
    ret.tail_ = secondHead;
    if (ret.tail_ != NULL) {
        while (ret.tail_->next != NULL)
            ret.tail_ = ret.tail_->next;
    }
    ret.length_ = oldLength - splitPoint;
    return ret;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in MP3.2
    while(splitPoint-1) {
      start = start->next;
      splitPoint--;
    }
    ListNode *newHead = start->next;
    start->next = nullptr;
    newHead->prev = nullptr;
    return newHead;
}

/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second  sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in MP3.2
    ListNode *mergedHead = nullptr;

    if (first == nullptr && second == nullptr) {
      return mergedHead;
    }
    else if (first == nullptr) {
      return second;
    }
    else if (second == nullptr) {
      return first;
    }

    if (second->data < first->data) {
      mergedHead = second;
      second = second->next;
    }
    else {
      mergedHead = first;
      first = first->next;
    }
    mergedHead->prev = nullptr;
    ListNode *iter = mergedHead;
    while (first != nullptr || second != nullptr) {
      if (first == nullptr) {
        iter->next = second;
        second->prev = iter;
        // cout<<"Segfault BIATCH"<<endl;
        // ListNode* temp = mergedHead;
        // while (temp!= nullptr) {
        //   cout<<temp->data<<endl;
        // }
        return mergedHead;
      }
      if (second == nullptr) {
        iter->next = first;
        first->prev = iter;
        return mergedHead;
      }

      if (second->data < first->data) {
        iter->next = second;
        second->prev = iter;
        second = second->next;
        iter = iter->next;

      }
      else {
        iter->next = first;
        first->prev = iter;
        first = first->next;
        iter = iter->next;
      }

    }

    return mergedHead;


}

/**
 * Sorts the current list by applying the Mergesort algorithm.
 */
template <typename T>
void List<T>::sort() {
    if (empty())
        return;
    head_ = mergesort(head_, length_);
    tail_ = head_;
    while (tail_->next != NULL)
        tail_ = tail_->next;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
    if (chainLength == 1) 
        return start;
    ListNode* mid = split(start, chainLength/2);
    return merge(mergesort(start, chainLength/2), mergesort(mid, chainLength- chainLength/2));
}
