/*
 * Implementation file for a clean and simple linked list
 * std::forward_list syntax
 */

// These are freebies, touch only if attempting doubly-linked list
// Tests may depend on these working!
template <typename T> ListNode<T> *MyList<T>::begin() { return head; }

template <typename T> MyList<T>::MyList() {
  num_elements = 0;
  head = new ListNode<T>;
  head->next = nullptr;
}

template <typename T> int MyList<T>::size() { return num_elements; }

template <typename T> T &MyList<T>::front() { return head->data; }

template <typename T> void MyList<T>::push_front(const T &value) {
  ListNode<T> *tmp = new ListNode<T>;
  tmp->data = value;
  tmp->next = head;
  head = tmp;
  num_elements++;
}

template <typename T> void MyList<T>::pop_front() {

  if (num_elements > 0) {
    ListNode<T> *tmp = head->next;
    delete head;
    head = tmp;
    num_elements--;
  }
}

// Implement each of the functions below
// PLEASE READ MyList.h carefully for details on each!
// Check out unit_tests/ and mem_tests/ for grading details

template <typename T> MyList<T>::~MyList() {
  clear();
  delete head;
}

template <typename T> MyList<T> &MyList<T>::operator=(const MyList<T> &rhs) {
  clear();
  ListNode<T> *positionp = head;
  ListNode<T> *datap = rhs.head;
  while (datap->next != nullptr) {
    insert(positionp, datap->data);
    positionp = positionp->next;
    datap = datap->next;
  }

  return *this;
}

template <typename T> MyList<T>::MyList(const MyList<T> &rhs) {
  head = new ListNode<T>;
  head->next = nullptr;

  *this = rhs;
  num_elements = rhs.num_elements;
}

template <typename T> void MyList<T>::assign(int count, const T &value) {
  clear();
  for (int iter = 0; iter < count; iter++) {
    push_front(value);
  }
  num_elements = count;
}

template <typename T> void MyList<T>::clear() {
  ListNode<T> *temp = head->next;
  while (temp != nullptr) {
    delete head;
    head = temp;
    temp = head->next;
  }

  num_elements = 0;
}

template <typename T>
void MyList<T>::insert(LinkedListIterator<T> iter, const T &value) {
  ListNode<T> *temp = new ListNode<T>;
  temp->data = iter->data;
  temp->next = iter->next;
  iter->data = value;
  iter->next = temp;
  num_elements++;
}

template <typename T> void MyList<T>::erase(LinkedListIterator<T> iter) {
  if (iter->next != nullptr) {
    ListNode<T> *temp = iter->next;
    iter->data = temp->data;
    iter->next = temp->next;
    delete temp;
    num_elements--;
  }
}

template <typename T> void MyList<T>::reverse() {
  ListNode<T> *curr = head;
  ListNode<T> *prev = nullptr;
  ListNode<T> *nextin;
  while (curr != nullptr) {
    nextin = curr->next;
    curr->next = prev;
    prev = curr;
    curr = nextin;
  }
  head = prev;
  ListNode<T> *temp = head;
  while (temp->next !=
         nullptr) { // not a nullptr reference, checking for nullptr.
    temp->data = temp->next->data;
    temp = temp->next;
  }
}

template <typename T> LinkedListIterator<T> MyList<T>::find(const T &value) {
  ListNode<T> *iter = head;
  while (iter->next != nullptr) {
    if (value == iter->data)
      return iter;
    iter = iter->next;
  }
  return nullptr;
}

template <typename T>
void MyList<T>::splice_after(ListNode<T> *splice_point, MyList<T> &source) {
  if(source.head == nullptr)
    return;
  ListNode<T>* temp = source.head;
  while(temp -> next != nullptr)
    temp = temp -> next;
  temp -> next = splice_point -> next;
  splice_point -> next = source.head;
  source.head = nullptr;
}
