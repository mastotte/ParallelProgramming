#include <atomic>

// this should be the same as RDekkers with the change that every
// relaxed access should be memory_order_relaxed.
// your job is to use the following MACRO to fix the mutex.
// Think about the reorderings allowed under TSO and where the fence
// needs to be placed.

#define FENCE asm volatile("mfence":::"memory");

class dekkers_mutex {
public:
  dekkers_mutex() {
    // implement me!
    wants_to_enter = new std::atomic<bool>[2];
    turn.store(0, std::memory_order_relaxed);
  }

  void lock(int tid) {
    // implement me!
    int other = tid == 0 ? 1 : 0;
    wants_to_enter[tid].store(true, std::memory_order_relaxed);
    FENCE;
    while (wants_to_enter[other].load(std::memory_order_relaxed)){
      if (turn.load(std::memory_order_relaxed) != tid){
        wants_to_enter[tid].store(false, std::memory_order_relaxed);
        while (turn.load(std::memory_order_relaxed) != tid){

        };
        wants_to_enter[tid].store(true, std::memory_order_relaxed);
      }
    }
  }

  void unlock(int tid) {
    // implement me!
    int other = tid == 0 ? 1 : 0;
    turn.store(other, std::memory_order_relaxed);
    wants_to_enter[tid].store(false, std::memory_order_relaxed);
  }

private:
  // Give me some private variables  
  std::atomic<bool> *wants_to_enter;
  std::atomic<int> turn;
};
