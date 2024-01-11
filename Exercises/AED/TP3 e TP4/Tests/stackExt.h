// Doxygen documentation included
/// @file stackExt.h

# include <iostream>
# include <stack>
using namespace std;

template <class T>
class StackExt {
	stack<T> values;
	stack<T> minimums;
public:
	StackExt() {};
	bool empty() const; 
	T& top();
	void pop();
	void push(const T& val);
	T& findMin();
};

//=============================================================================
// Exercise 3: StackExt class
//=============================================================================
/**
 * Indicates if the stack is empty or not.
 * Similar from stack.empty() from STL.
 * @param[out] bool
*/
template <class T>
bool StackExt<T>::empty() const {
    return values.empty();
}

/**
 * Returns the object at the top of the stack, without deleting it.
 * Similar from stack.top() from STL.
 * @param[out] T&
*/
template <class T>
T& StackExt<T>::top() {
    return values.top();
}

/**
 * Deletes the object at the top of the stack, without returning it.
 * Similar from stack.pop() from STL, but it also alters the minimums stack.
*/
template <class T>
void StackExt<T>::pop() {
    T top = values.top();
    values.pop();
    if (top == minimums.top()) minimums.pop();
}

/**
 * Inserts the object into the top of the stack.
 * Similar from stack.push() from STL, but it also alters the minimums stack.
 * @param[in] T&
*/
template <class T>
void StackExt<T>::push(const T& val) {
    values.push(val);
    if (minimums.empty()) minimums.push(val);
    else if (val <= minimums.top()) minimums.push(val);
}

/**
 * Indicates the minimum value in the whole stack.
 * It uses another stack to keep track of every pop() and push().
 * @param[out] T&
*/
template <class T>
T& StackExt<T>::findMin() {
    return minimums.top();
}
