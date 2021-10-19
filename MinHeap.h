#ifndef MINHEAP_INCLUDED
#define MINHEAP_INCLUDED
#include <exception>
#include <stdexcept>
#include <utility>
#include <vector>

template<class T>
class MinHeap {
public:
    MinHeap(int d);
    /* Constructor that builds a d-ary Min Heap
       This should work for any d >= 2,
       but doesn't have to do anything for smaller d.*/

    ~MinHeap(){};

    void add(T item, int priority);
    /* adds the item to the heap, with the given priority. */

    const T& peek();
    /* returns the element with smallest priority.
       Break ties however you wish.
       Throws an exception if the heap is empty. */

    void remove();
    /* removes the element with smallest priority.
       Break ties however you wish.
       Throws an exception if the heap is empty. */

    bool isEmpty();
    /* returns true iff there are no elements on the heap. */

private:
    // Vector of pairs holding type of object and its priority
    std::vector<std::pair<T, int>> heap_vector;

    // Holding what d_ary heap
    int d_ary;

    // Find index of parent given child_index
    int parent_of(int child_index);

    // Find index of smallest child given parent_index;
    int smallest_child(int parent_index);
};

// Constructor
template<class T>
MinHeap<T>::MinHeap(int d) {
    d_ary = d;
}

// Finding parent index
template<class T>
int MinHeap<T>::parent_of(int child_index) {
    return (child_index - 1) / d_ary;
}

// Finding smallest child index
template<class T>
int MinHeap<T>::smallest_child(int parent_index) {

    // Checking if parent index has children
    int smallest = (d_ary * parent_index) + 1;
    if (smallest >= int(heap_vector.size())) {
        return parent_index;
    }

    // Comparing all children priority of top node and seeing which one is smallest
    for (int i = 1; i < d_ary; i += 1) {
        // Finding which child we're looking at
        int childx_index = (d_ary * parent_index) + 1 + i;
        // Breaks loop if child becomes out of range
        if (childx_index >= int(heap_vector.size())) {
            break;
        }
        // Checking priority at childx vs at variable smallest_child
        // If smaller, then it is the new smallest_child
        if (heap_vector[childx_index].second < heap_vector[smallest].second) {
            smallest = childx_index;
        }
    }

    return smallest;
}

// Add T object
template<class T>
void MinHeap<T>::add(T item, int priority) {

    // Pushing object initially to back of vector;
    heap_vector.push_back(std::make_pair(item, priority));

    // Current index of object
    int current_index = heap_vector.size() - 1;

    // First index to compare is parent of inserted position
    int index_to_compare = parent_of(current_index);

    // While object priority is less than index_to_compare, will swap
    // the two elements and update index_to_compare to its parent's index
    while (priority <= heap_vector[index_to_compare].second && current_index != 0) {
        // If priority is same see which string is alphabetically smaller
        if (priority == heap_vector[index_to_compare].second && heap_vector[index_to_compare].first < item) {
            break;
        }
        // Stores index to swap down in temp
        std::pair<T, int> temp = heap_vector[index_to_compare];
        // Swaps object at current_index with its parent at index_to_compare
        heap_vector[index_to_compare] = heap_vector[current_index];
        heap_vector[current_index] = temp;
        // Updates current_index to former index_to_compare
        current_index = index_to_compare;
        // Updates index_to_compare to new parent;
        index_to_compare = parent_of(current_index);
    }
}

// Peek top priority object
template<class T>
const T& MinHeap<T>::peek() {

    // Throws exception if heap is empty
    if (heap_vector.empty()) {
        throw std::logic_error("Heap is empty");
    }

    // If two priorities tied, chooses smaller string alphabetically
    for (int i = 1; i <= d_ary; i += 1) {
        if (i > int(heap_vector.size() - 1)) {
            break;
        }
        if (heap_vector[0].second == heap_vector[i].second && heap_vector[i].first < heap_vector[0].first) {
            std::pair<T, int> temp = heap_vector[0];
            heap_vector[0] = heap_vector[i];
            heap_vector[i] = temp;
        }
    }

    // Returns first item of index 0 in vector
    return heap_vector[0].first;
}

// Remove first element of vector but maintain heap status
template<class T>
void MinHeap<T>::remove() {

    // Throws exception if heap is empty
    if (isEmpty()) {
        throw std::logic_error("Heap is empty");
    }

    // Replacing first element with last element and popping last element
    heap_vector[0] = heap_vector[heap_vector.size() - 1];
    heap_vector.pop_back();

    // Trickling down new element at top
    // Current index of object
    int current_index = 0;

    // First index to compare is smallest child of top node
    int index_to_compare = smallest_child(0);

    // While there is a child priority lower than current priority, swaps
    // the two elements and updates index_to_compare to its child's index
    while (heap_vector[index_to_compare].second < heap_vector[current_index].second) {

        // Stores index to swap down in temp
        std::pair<T, int> temp = heap_vector[index_to_compare];
        // Swaps object at current_index with its parent at index_to_compare
        heap_vector[index_to_compare] = heap_vector[current_index];
        heap_vector[current_index] = temp;
        // Updates current_index to former index_to_compare
        current_index = index_to_compare;
        // Updates index_to_compare to new smallest child;
        index_to_compare = smallest_child(current_index);
    }
}

// Checks if heap is empty
template<class T>
bool MinHeap<T>::isEmpty() {
    return heap_vector.empty();
}

#endif