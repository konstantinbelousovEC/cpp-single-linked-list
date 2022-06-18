#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <initializer_list>
#include <algorithm>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
    template <typename ValueType>
        class BasicIterator {
            friend class SingleLinkedList;

            explicit BasicIterator(Node* node)
            : node_(node) {
            }

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Type;
            using difference_type = std::ptrdiff_t;
            using pointer = ValueType*;
            using reference = ValueType&;

            BasicIterator() = default;

            BasicIterator(const BasicIterator<Type>& other) noexcept {
                node_ = other.node_;
            }

            BasicIterator& operator=(const BasicIterator& rhs) = default;

            [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
                return this->node_ == rhs.node_;
            }

            [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
                return !(*this == rhs);
            }

            [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
                return this->node_ == rhs.node_;
            }

            [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
                return !(*this == rhs);
            }

            BasicIterator& operator++() noexcept {
                assert(node_ != nullptr);
                node_ = this->node_->next_node;
                return *this;
            }

            BasicIterator operator++(int) noexcept {
                auto old_value(*this);
                ++(*this);
                return old_value;
            }

            [[nodiscard]] reference operator*() const noexcept {
                assert(node_ != nullptr);
                return node_->value;
            }

            [[nodiscard]] pointer operator->() const noexcept {
                assert(node_ != nullptr);
                return &(node_->value);
            }

        private:
            Node* node_ = nullptr;
        };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;
    
    SingleLinkedList() = default;
    
    SingleLinkedList(std::initializer_list<Type> values) {
        Copy_and_Swap(values);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        Copy_and_Swap(other);
    }
    
    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        SingleLinkedList tmp(rhs);
        swap(tmp);
        return *this;
    }
    
    ~SingleLinkedList() {
        Clear();
    }
    
    void Clear() noexcept;
    void PushFront(const Type& value);
    void PopFront() noexcept;
    Iterator InsertAfter(ConstIterator pos, const Type& value);
    Iterator EraseAfter(ConstIterator pos) noexcept;
    void swap(SingleLinkedList& other) noexcept;
    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0 ? true : false;
    }

    [[nodiscard]] Iterator begin() noexcept {
        return BasicIterator<Type>(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return BasicIterator<Type>(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return BasicIterator<Type>(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return BasicIterator<Type>(nullptr);
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        return BasicIterator<Type>(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return BasicIterator<Type>(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }

private:
    template <typename Iterable>
    void Copy_and_Swap(const Iterable& container);
    
    Node head_ = {};
    size_t size_ = 0;
};

template <typename Type>
void SingleLinkedList<Type>::Clear() noexcept {
    while (head_.next_node != nullptr) {
        Node* tmp = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = tmp;
    }
    size_ = 0u;
}

template <typename Type>
void SingleLinkedList<Type>::PushFront(const Type& value) {
    head_.next_node = new Node(value, head_.next_node);
    ++size_;
}

template <typename Type>
void SingleLinkedList<Type>::PopFront() noexcept {
    assert(!IsEmpty());
    Node* tmp = head_.next_node->next_node;
    delete head_.next_node;
    head_.next_node = tmp;
    size_--;
}

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::InsertAfter(ConstIterator pos, const Type& value) {
    assert(pos.node_ != nullptr);
    pos.node_->next_node = new Node(value, pos.node_->next_node);
    size_++;
    return BasicIterator<Type>(pos.node_->next_node);
}

template <typename Type>
typename SingleLinkedList<Type>::Iterator SingleLinkedList<Type>::EraseAfter(ConstIterator pos) noexcept {
    assert(pos.node_ != nullptr);
    assert(pos.node_->next_node != nullptr);
    Node* tmp = pos.node_->next_node->next_node;
    delete pos.node_->next_node;
    pos.node_->next_node = tmp;
    return BasicIterator<Type>(pos.node_->next_node);
}

template <typename Type>
void SingleLinkedList<Type>::swap(SingleLinkedList& other) noexcept {
    std::swap(head_.next_node, other.head_.next_node);
    std::swap(size_, other.size_);
}

template <typename Type>
template <typename Iterable>
void SingleLinkedList<Type>::Copy_and_Swap(const Iterable& container) {
    SingleLinkedList tmp;
    auto position = tmp.before_begin();
    for (Type elem : container) {
        position = tmp.InsertAfter(position, elem);
    }
    swap(tmp);
}

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs > lhs);
}
