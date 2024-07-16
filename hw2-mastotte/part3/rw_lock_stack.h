#include <shared_mutex>
using namespace std;

class Llist_node {
 public:  
  Llist_node(int d) {   
    data = d;
    next = NULL;
  }
  
  int data;
  Llist_node *next;
};

class CSE113_Stack {
 public:
  
  CSE113_Stack() {
    start = NULL;
  }
  
  int pop() {
    s_m.lock();
    if (start == NULL) {
      s_m.unlock();
      return -1;
    }

    if (start->next == NULL) {
      int ret = start->data;
      delete start;
      start = NULL;
      s_m.unlock();
      return ret;
    }

    Llist_node *current = start->next;
    Llist_node *previous = start;
    while (current->next != NULL) {
      previous = current;
      current = current->next;
    }

    int ret = current->data;
    previous->next = NULL;
    delete current;
    s_m.unlock();
    return ret;
  }

  
  int peek() {
    s_m.lock_shared();
    if (start == NULL) {
      s_m.unlock_shared();
      return -1;
    }
    
    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }
    int result = current->data;
    s_m.unlock_shared();
    return result;  
  }
  
  void push(int p) {
    s_m.lock();
    if (start == NULL) {

      start = new Llist_node(p);
      s_m.unlock();
      return;
    }

    Llist_node *current = start;
    while (current->next != NULL) {
      current = current->next;
    }

    current->next = new Llist_node(p);
    s_m.unlock();
  }

 private:
  shared_mutex s_m;
  Llist_node *start;  
};

