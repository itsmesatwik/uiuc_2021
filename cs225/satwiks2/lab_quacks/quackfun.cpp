/**
 * @file quackfun.cpp
 * This is where you will implement the required functions for the
 *  stacks and queues portion of the lab.
 */

namespace QuackFun {

/**
 * Sums items in a stack.
 * @param s A stack holding values to sum.
 * @return The sum of all the elements in the stack, leaving the original
 *  stack in the same state (unchanged).
 *
 * @note You may modify the stack as long as you restore it to its original
 *  values.
 * @note You may use only two local variables of type T in your function.
 *  Note that this function is templatized on the stack's type, so stacks of
 *  objects overloading the + operator can be summed.
 * @note We are using the Standard Template Library (STL) stack in this
 *  problem. Its pop function works a bit differently from the stack we
 *  built. Try searching for "stl stack" to learn how to use it.
 * @hint Think recursively!
 */
template <typename T>
T sum(stack<T>& s)
{
  if (s.size() == 1) {
    T elem = s.top();
    s.pop();
    T sum_ = 0;
    sum_ += elem;
    s.push(elem);
    return sum_;
  }
  T sum_ = 0;
  T elem = s.top();
  s.pop();
  sum_ += elem;
  sum_ += sum(s);
  s.push(elem);
  return sum_;
    // Your code here
   // stub return value (0 for primitive types). Change this!
                // Note: T() is the default value for objects, and 0 for
                // primitive types
}

/**
 * Checks whether the given string (stored in a queue) has balanced brackets.
 * A string will consist of
 * square bracket characters, [, ], and other characters. This function will return
 * true if and only if the square bracket characters in the given
 * string are balanced. For this to be true,
 * all brackets must be matched up correctly, with no extra, hanging, or unmatched
 * brackets. For example, the string "[hello][]" is balanced, "[[][[]a]]" is balanced,
 * "[]]" is unbalanced, "][" is unbalanced, and "))))[cs225]" is balanced.
 *
 * For this function, you may only create a single local variable of type stack<char>!
 * No other stack or queue local objects may be declared. Note that you may still
 * declare and use other local variables of primitive types.
 *
 * @param input The queue representation of a string to check for balanced brackets in
 * @return Whether the input string had balanced brackets
 */
bool isBalanced(queue<char> input)
{

    // @TODO: Make less optimistic
    stack<char> a;
    int l_count = 0;
    int r_count = 0;
    while (!input.empty()) {
      char c = input.front();
      input.pop();
      if (c == '[') {
        l_count++;
      }
      if (c == ']') {
        r_count++;
      }

      if (r_count > l_count) {
        return false;
      }
    }

    if (l_count == r_count) {
      return true;
    }
    return false;
}

/**
 * Reverses even sized blocks of items in the queue. Blocks start at size
 * one and increase for each subsequent block.
 * @param q A queue of items to be scrambled
 *
 * @note Any "leftover" numbers should be handled as if their block was
 *  complete.
 * @note We are using the Standard Template Library (STL) queue in this
 *  problem. Its pop function works a bit differently from the stack we
 *  built. Try searching for "stl stack" to learn how to use it.
 * @hint You'll want to make a local stack variable.
 */
template <typename T>
void scramble(queue<T>& q)
{
    stack<T> s;
    /*queue<T> q2;
    bool flip = false;
    unsigned long index = 0;
    for (unsigned long k = 1; k< q.size(); k++) {
        if (index >= q.size())
            break;
      for (unsigned long i = 0; i < k; i++) {
        if (!q.empty()) {
          T elem = q.front();
          q.pop();
          if (flip) {
            s.push(elem);
          } else {
            q2.push(elem);
          }
          index++;
        } 
        else {
            break;
        }
      }

      if (flip) {
        while (!s.empty()) {
          T elem = s.top();
          s.pop();
          q2.push(elem);
        }
      }

      flip = !flip;
    }*/

    queue<T> q2;
    int flip_size = 1;
    bool flip = false;
    while (!q.empty()) {
        if (!flip) {
            for (int i = 0; i < flip_size; i++) {
                if(!q.empty()) {
                    q2.push(q.front());
                    q.pop();
                }
            }
        }
        else {
            for (int i = 0; i < flip_size; i++) {
               if(!q.empty()) {
                   s.push(q.front());
                   q.pop();
               }
            }
            while (!s.empty()) {
                q2.push(s.top());
                s.pop();
            }
        }

        flip_size++;
        flip = !flip;
    }

    while (!q2.empty()) {
        q.push(q2.front());
        q2.pop();
    }

    // optional: queue<T> q2;

    // Your code here
}

/**
 * @return true if the parameter stack and queue contain only elements of
 *  exactly the same values in exactly the same order; false, otherwise.
 *
 * @note You may assume the stack and queue contain the same number of items!
 * @note The back of the queue corresponds to the top of the stack!
 * @note There are restrictions for writing this function.
 * - Your function may not use any loops
 * - In your function you may only declare ONE local boolean variable to use in
 *   your return statement, and you may only declare TWO local variables of
 *   parametrized type T to use however you wish.
 * - No other local variables can be used.
 * - After execution of verifySame, the stack and queue must be unchanged. Be
 *   sure to comment your code VERY well.
 */
template <typename T>
bool verifySame(stack<T>& s, queue<T>& q)
{
    bool retval = true; // optional
    // T temp1; // rename me
    // T temp2; // rename :)
    T stack_t;
    T queue_t;
    if (s.size() == 1) {
        stack_t = s.top();
        queue_t = q.front();
        q.pop();
        q.push(queue_t);
        retval = (stack_t == queue_t);
        return retval;
    }

    stack_t = s.top();
    s.pop();
    retval = retval && verifySame(s,q);
    queue_t = q.front();
    q.pop();
    retval = retval && (stack_t == queue_t);
    s.push(stack_t);
    q.push(queue_t);

    // Your code here

    return retval;
}

}
