#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cassert>

// References:
// - Prob 1.3 from the problem session 1
//   https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-spring-2020/resources/mit6_006s20_prob1/
// - https://en.cppreference.com/w/cpp/container/deque

// Run the code to visualize the algorithm 🐢

/*
Design a data structure to store a sequence of items that supports worst-case
O(1)-time index lookup, as well as amortized O(1)-time insertion and removal
at both ends. Your data structure should use O(n) space to store n items
*/

// We will implement a "double ended queue" (deque)

/*
Deques are a data structure that combines the benefits of queues and stacks.

The deques are characterized for its O(1)-time to remove and push elements
at the both ends, and it can be implemented in many ways.

Deques can be implemented using linked lists, but that will give us a O(n)-time
for random index access.

So we will use an array implementation to solve this problem, that will give us:
 - O(1)-time for insertion and deletion at both ends
 - O(1)-time for index lookup
 - O(n)-space to store n items
*/

template <typename T>
class Deque {
  private:
    std::size_t m_offset = 0;
    std::size_t m_size = 0;
    std::size_t m_capacity = 0;
    T* m_data = nullptr;
  public:
    Deque(std::size_t capacity);
    Deque();
    void push_back(T element);    // O(1)
    void push_front(T element);   // O(1)
    void pop_back();     // O(1)
    void pop_front();    // O(1)
    void grow_back(std::size_t size);  // amortized O(1)
    void grow_front(std::size_t size); // amortized O(1)
    T& operator[](std::size_t index);  // O(1)
    void pretty_print() const;    // O(n)
    const T& operator[](std::size_t index) const;
    std::size_t size() const;
    std::size_t capacity() const;
    bool empty() const;
    ~Deque();
};

// to print the array after push_back() & push_front()
#define PRINT_AFTER_PUSHING true
// to print the array after pop_back() & pop_front()
#define PRINT_AFTER_POPPING true
int main() {
    // try this out
    int initial_capacity = 10;
    Deque<char> deque(initial_capacity);
    deque.push_back('w');
    deque.push_back('o');
    deque.push_back('r');
    deque.push_back('l');
    deque.push_back('d');

    deque.push_front(' ');
    deque.push_front('o');
    deque.push_front('l');
    deque.push_front('l');
    deque.push_front('e');
    deque.push_front('h');

    deque[0] = 'H';
    deque.pretty_print();
    return 0;
}

/* Implementations of Deque methods */
template <typename T>
Deque<T>::Deque(std::size_t initial_capacity) {
    m_capacity = initial_capacity;
    m_data = new T[m_capacity];
    m_offset = initial_capacity / 3;
}

template <typename T>
Deque<T>::Deque() { }

template <typename T>
void Deque<T>::push_back(T element) {
    if (m_offset + m_size == m_capacity) {
        this->grow_back(m_size + 1);
    }
    m_data[m_offset + m_size] = element;
    m_size++;
    if (PRINT_AFTER_PUSHING) this->pretty_print();
}

template <typename T>
void Deque<T>::push_front(T element) {
    if (m_offset == 0) {
        this->grow_front(m_size + 1);
    }
    m_data[m_offset - 1] = element;
    m_offset--;
    m_size++;
    if (PRINT_AFTER_PUSHING) this->pretty_print();
}

template <typename T>
void Deque<T>::pop_back() {
    if (m_size == 0) {
        throw std::runtime_error("couldn't pop_back from empty deque");
    }
    m_size--;
    if (PRINT_AFTER_POPPING) this->pretty_print();
}

template <typename T>
void Deque<T>::pop_front() {
    if (m_size == 0) {
        throw std::runtime_error("couldn't pop_front from empty deque");
    }
    m_offset++;
    m_size--;
    if (PRINT_AFTER_POPPING) this->pretty_print();
}

/*
Expands the array capacity and copies the elements to the middle
*/
template <typename T>
void Deque<T>::grow_front(std::size_t size) {
    assert(size > 0 && "size must be positive");
    int new_capacity = m_capacity + size;
    T* new_data = new T[new_capacity];
    std::size_t new_offset = m_offset + size;
    for (std::size_t i = 0; i < m_size; i++) {
        new_data[new_offset + i] = m_data[m_offset + i];
    }
    delete[] m_data;
    m_data = new_data;
    m_offset = new_offset;
    m_capacity = new_capacity;
}

template <typename T>
void Deque<T>::grow_back(std::size_t size) {
    assert(size > 0 && "size must be positive");
    int new_capacity = m_capacity + size;
    T* new_data = new T[new_capacity];
    for (std::size_t i = 0; i < m_size; i++) {
        new_data[m_offset + i] = m_data[m_offset + i];
    }
    delete[] m_data;
    m_data = new_data;
    m_capacity = new_capacity;
}

/*
This is to visualize how the array grows at the front and back
Only works for types implemented by std::to_string,
like ints, floats, strings, etc.
*/
template <typename T>
void Deque<T>::pretty_print() const {
    std::vector<int> lengths;
    int sum_of_lengths = 0;

    for (std::size_t i = 0; i < m_size; i++) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << m_data[m_offset + i];
        int len = ss.str().length();
        lengths.push_back(len);
        sum_of_lengths += len;
    }

    int empty_spaces   = (m_capacity - m_size) * 2;
    int ocupied_spaces = sum_of_lengths + lengths.size();
    std::string fill;
    for (int i = 0; i < empty_spaces + ocupied_spaces + 1; i++) {
        fill += "─";
    }
    std::cout << "┌" << fill << "┐\n";
    std::cout << "│ ";
    for (std::size_t i = 0; i < m_capacity; i++) {
        if (i < m_offset || i > m_offset + m_size - 1) {
            std::cout << "░ ";
        }
        else {
            std::cout << std::fixed << std::setprecision(2) << m_data[i] << " ";
        }
    }
    std::cout << "│\n";
    std::cout << "└" << fill << "┘\n";
}

/* Miscelanious */

// Random index access O(1)
template <typename T>
T& Deque<T>::operator[](std::size_t index) {
    if (index > m_size - 1) {
        throw std::out_of_range("index out of range");
    }
    return m_data[m_offset + index];
}

template <typename T>
const T& Deque<T>::operator[](std::size_t index) const {
    if (index > m_size - 1) {
        throw std::out_of_range("index out of range");
    }
    return m_data[m_offset + index];
}

template <typename T>
std::size_t Deque<T>::size() const {
    return m_size;
}

template <typename T>
std::size_t Deque<T>::capacity() const {
    return m_capacity;
}

template <typename T>
Deque<T>::~Deque() {
    if (m_data) delete[] m_data;
}

template <typename T>
bool Deque<T>::empty() const {
    return m_size == 0;
}
