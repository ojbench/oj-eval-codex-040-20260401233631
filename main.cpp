// Problem 040 - Doubly linked list with shared_ptr/weak_ptr semantics
// Implements the template from README with the TODO(student) parts filled.

#include <iostream>
#include <memory>
#include <optional>

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(const T& val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length;

public:
    class iterator {
    private:
        std::shared_ptr<Node> current;

    public:
        iterator(std::shared_ptr<Node> node) : current(std::move(node)) {}

        T& operator*() {
            return current->value;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            if (current) current = current->next;
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    DoublyLinkedList() : head(nullptr), length(0) {}

    bool empty() const { return length == 0; }
    size_t size() const { return length; }

    void push_front(const T& value) {
        auto node = std::make_shared<Node>(value);
        node->next = head;
        if (head) {
            head->prev = node;
        } else {
            tail = node;
        }
        head = node;
        ++length;
    }

    void push_back(const T& value) {
        auto node = std::make_shared<Node>(value);
        auto t = tail.lock();
        if (!t) {
            head = node;
            tail = node;
        } else {
            t->next = node;
            node->prev = t;
            tail = node;
        }
        ++length;
    }

    std::optional<T> pop_front() {
        if (!head) return std::nullopt;
        auto old = head;
        auto val = old->value;
        head = old->next;
        if (head) {
            head->prev.reset();
        } else {
            tail.reset();
        }
        --length;
        return val;
    }

    std::optional<T> pop_back() {
        if (empty()) return std::nullopt;
        auto t = tail.lock();
        auto val = t->value;
        auto prev = t->prev.lock();
        if (prev) {
            prev->next.reset();
            tail = prev;
        } else {
            head.reset();
            tail.reset();
        }
        --length;
        return val;
    }

    class iterator;
    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};
