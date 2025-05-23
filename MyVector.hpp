/* Define all your MyVector-related functions here.
 * You do not need to include the h file.
 * It included this file at the bottom.
 */

// We're giving you these functions for free
// and as a guide for the syntax.
#include "MyVector.h"
template <typename T> MyVector<T>::~MyVector() {
  clear();
  delete[] data;
}

template <typename T> T &MyVector<T>::operator[](int index) {
  return data[index]; // no bounds checking!!
}

template <typename T> void MyVector<T>::push_back(const T &value) {
  if (num_elements > 0) {
    if (num_elements == max_elements) // double capacity if full
      reserve(max_elements * 2);
  } else {
    if (num_elements == max_elements)
      reserve(1); // edge case for when max max_elements == 0
  }

  data[num_elements] = value;
  num_elements++;
}

template <typename T> void MyVector<T>::pop_back() {
  if (num_elements > 0)
    num_elements--;
}

template <typename T> void MyVector<T>::reserve(int new_max_elements) {

  T *temp = new T[new_max_elements];
  for (int k = 0; k < num_elements; k++)
    temp[k] = data[k];
  max_elements = new_max_elements;
  delete[] data;
  data = temp;
  temp = nullptr;
}

template <typename T> int MyVector<T>::size() { return num_elements; }

template <typename T> int MyVector<T>::capacity() { return max_elements; }

template <typename T> T &MyVector<T>::front() { return data[0]; }

template <typename T> T &MyVector<T>::back() { return data[num_elements - 1]; }

// Another example: remember when writing an implementation in hpp,
// return type first, then scope just before the function name.
template <typename T> T &MyVector<T>::at(int index) {
  // Define the function here. Write this one yourself!
  if (0 <= index && index < num_elements) {
    return data[index];
  } else
    throw std::out_of_range("At: index out of range");
}

/*
Also implement the following functions:

/// Default Member Functions ///
* ~MyVector()
* const MyVector<T>& operator=( const MyVector<T> &rhs )
* MyVector( const MyVector<T> &rhs )

/// Operations ///
* void insert(int i, const T& x)
* void erase(int i)
* bool find(const T& x)

/// Auxilliary ///
* void shrink_to_fit()

/// New Operations ///
* void swap(MyVector<T> & other)

*/

template <typename T> void MyVector<T>::shrink_to_fit() {
  T *tmp = new T[num_elements];
  for (int iter = 0; iter < num_elements; iter++) {
    tmp[iter] = data[iter];
  }
  delete[] data;
  data = tmp;
  tmp = nullptr;
  max_elements = num_elements;
}

template <typename T>
const MyVector<T> &MyVector<T>::operator=(const MyVector<T> &rhs) {
  delete[] data;
  data = new T[rhs.max_elements];
  for (int iter = 0; iter < rhs.num_elements; iter++) {
    data[iter] = rhs.data[iter];
  }
  num_elements = rhs.num_elements;
  max_elements = rhs.max_elements;
  return *this;
}

template <typename T> MyVector<T>::MyVector(const MyVector<T> &rhs) {
  *this = rhs;
}

template <typename T>
void MyVector<T>::insert(ArrayListIterator index, const T &value) {
  if (0 <= index && index <= max_elements) {
    if (num_elements == max_elements)
      reserve(max_elements * 2);
    for (int iter = num_elements; iter > index; iter--)
      data[iter] = data[iter - 1];
    data[index] = value;
    num_elements++;
  } else
    throw std::out_of_range(
        "Insert Func: Index was out of range for the vector function.");
}

template <typename T> void MyVector<T>::erase(ArrayListIterator index) {
  if (0 <= index && index < num_elements) {
    if (num_elements > 0) {
      for (int iter = index; iter < num_elements - 1; iter++)
        data[iter] = data[iter + 1];
      num_elements--;
    }
  } else
    throw std::out_of_range(
        "Erase Func: Index was out of range for the vector function.");
}

// If `value` in list, returns the index of the first occurence of `value`
// If `value` not in list, returns -1
template <typename T> int MyVector<T>::find(const T &value) {

  for (int iter = 0; iter < num_elements; iter++) {
    if (value == data[iter]) {
      return iter;
    }
  }

  return -1;
}

template <typename T> void MyVector<T>::clear() { num_elements = 0; }

template <typename T> void MyVector<T>::swap(MyVector<T> &other) {
  T *temp = data;
  data = other.data;
  other.data = temp;
  temp = nullptr;
  int temp2 = num_elements;
  num_elements = other.num_elements;
  other.num_elements = temp2;
  temp2 = max_elements;
  max_elements = other.max_elements;
  other.max_elements = temp2;
}
