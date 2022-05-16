// -*- coding:utf-8-unix -*-
/*!
    @file       var_list.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-24

    @brief      Double linked list what can store elements of type T and derived types
    @description
                T _must_ have virtual destructor!!!
                Usage:
                var_list<Widget> list;
                Widget& w1 = list.emplace_back<Widget>(parent, rect1);
                WidgetInput& w2 = list.emplace_back<WidgetInput>(parent, rect2, "text");

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__VAR_LIST_H
#define file__VAR_LIST_H

#include <type_traits> // std::has_virtual_destructor<>, etc.
#include <utility> // std::forward
#include <cstddef> // offsetof
#include <cstdlib> // malloc, free

template<typename T>
class var_list
{
    typedef var_list self_t;

    struct Node
    {
        Node* prev;
        Node* next;

        char dummy; // here is payload placed
    };

    template<typename U>
    static constexpr size_t calcNodeSize()
    {
        return offsetof(Node, dummy) + sizeof(U);
    }

    static Node* nodeFromDataPtr(void* ptr)
    {
        return (Node*)(void*)((char*)ptr - offsetof(Node, dummy));
    }

    static Node const* nodeFromDataPtr(void const* ptr)
    {
        return (Node*)(void*)((char*)ptr - offsetof(Node, dummy));
    }

    template<typename TPayload>
    static TPayload const* nodePayload(Node const* n)
    {
        return (TPayload const*)(void const*)((char const*)(void const*)n + offsetof(Node, dummy));
    }

    template<typename TPayload>
    static TPayload* nodePayload(Node* n)
    {
        return const_cast<TPayload*>(nodePayload<TPayload>((Node const*)n));
    }

    static void destroyNode(Node* n)
    {
        static_assert(std::has_virtual_destructor<T>::value);
        T* base = nodePayload<T>(n);
        base->~T();
        free(n);
    }

    template<typename U>
    static Node* allocate()
    {
        static_assert(std::has_virtual_destructor<T>::value);
        static_assert(std::is_base_of<T, U>::value);

        Node* p = (Node*)malloc(calcNodeSize<U>());

        p->prev = nullptr;
        p->next = nullptr;

        // inplace c'tor
        new (nodePayload<U>(p)) U();

        return p;
    }

    template<typename U, typename... Args>
    static Node* allocate(Args&&... args)
    {
        static_assert(std::has_virtual_destructor<T>::value);
        static_assert(std::is_base_of<T, U>::value);

        Node* p = (Node*)malloc(calcNodeSize<U>());

        p->prev = nullptr;
        p->next = nullptr;

        // inplace c'tor
        new(nodePayload<U>(p)) U(std::forward<Args>(args)...);

        return p;
    }

    void removeNode(Node* n)
    {
        assert(n);

        auto prev = n->prev;
        auto next = n->next;
        destroyNode(n);
        if (prev) {
            prev->next = next;
        }
        else { // removed head
            m_head = next;
        }

        if (next) {
            next->prev = prev;
        }
        else { // removed tail
            m_tail = prev;
        }

        --m_size;
    }

    /// insert newly created node to tail
    void insertBack(Node* n)
    {
        if (empty()) {
            m_head = m_tail = n;
        }
        else {
            n->prev = m_tail;
            n->next = nullptr;
            m_tail->next = n;
            m_tail = n;
        }

        ++m_size;
    }

    /// insert newly created node to front
    void insertFront(Node* n)
    {
        if (empty()) {
            m_head = m_tail = n;
        }
        else {
            n->next = m_head;
            n->prev = nullptr;
            m_head->prev = n;
            m_head = n;
        }

        ++m_size;
    }

    void insertNodeBefore(Node* n, Node* before)
    {
        if (before) {
            n->next = before;
            n->prev = before->prev;
            before->prev = n;

            if (m_head == before)
                m_head = n;

            ++m_size;
        }
        else { // insert back
            insertBack(n);
        }
    }


public:
    var_list() = default;
    var_list(self_t const&) = delete; // lazy to implement
    self_t& operator=(self_t const&) = delete; // lazy to implement

    var_list(self_t&& o)
    {
        swap(o);
    }

    void operator=(self_t&& o)
    {
        clear();
        swap(o);
    }

    ~var_list()
    {
        clear();
    }

    bool empty() const { return size() == 0; }
    size_t size() const { return m_size; }

    void clear()
    {
        auto curr = m_head;
        Node* next;
        while (curr)
        {
            next = curr->next;
            destroyNode(curr);
            curr = next;
        }
        m_head = m_tail = nullptr;
        m_size = 0;
    }

    T& front() { return *nodePayload<T>(m_head); }
    T const& front() const { return nodePayload<T>(m_head); }

    T& back() { return *nodePayload<T>(m_tail); }
    T const& back() const { return nodePayload<T>(m_tail); }

    template<typename U, typename... Args>
    U& emplace_back(Args&&... args) {
        Node* n = allocate<U>(std::forward<Args>(args)...);
        insertBack(n);
        return *nodePayload<U>(n);
    }

    template<typename U, typename... Args>
    U& emplace_front(Args&&... args) {
        Node* n = allocate<U>(std::forward<Args>(args)...);
        insertFront(n);
        return *nodePayload<U>(n);
    }

    //
    // iterators
    //

    // -------------------------------------------------------------- const_iterator
    class const_iterator// : public
    {
        friend class var_list;
        typedef const_iterator self_t;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = int;
        using pointer = T const*;
        using reference = T const&;

        reference operator * () const noexcept { return payload(); }
        pointer operator->() const noexcept { return &payload(); }
        self_t& operator++() noexcept { toNext(); return *this; }
        self_t& operator++(int) noexcept { auto tmp = *this; toNext(); return tmp; }
        self_t& operator--() noexcept { toPrev(); return *this; }
        self_t& operator--(int) noexcept { auto tmp = *this; toPrev(); return tmp; }
        bool operator==(const self_t& o) const noexcept { return m_node == o.m_node; }
        bool operator!=(const self_t& o) const noexcept { return m_node != o.m_node; }

        bool valid() const { return m_node != nullptr; }

    protected:
        const_iterator(Node const* n) : m_node(n) {}
        Node const* m_node = nullptr;
        void toNext() { m_node = m_node->next; }
        void toPrev() { m_node = m_node->prev; }
        reference payload() const { return *nodePayload<T>(m_node); }
    };

    // -------------------------------------------------------------- iterator
    class iterator : public const_iterator
    {
        friend class var_list;
        typedef iterator self_t;
        typedef const_iterator base_t;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = int;
        using pointer = T*;
        using reference = T&;

        reference operator * () noexcept { return payload(); }
        pointer operator->() noexcept { return &payload(); }
        self_t& operator++() noexcept { base_t::toNext(); return *this; }
        self_t& operator++(int) noexcept { auto tmp = *this; base_t::toNext(); return tmp; }
        self_t& operator--() noexcept { base_t::toPrev(); return *this; }
        self_t& operator--(int) noexcept { auto tmp = *this; base_t::toPrev(); return tmp; }
        bool operator==(const self_t& o) const noexcept { return base_t::operator==(o); }
        bool operator!=(const self_t& o) const noexcept { return base_t::operator!=(o); }

    protected:
        Node* node() { return const_cast<Node*>(base_t::m_node); }
        iterator(Node* n) : const_iterator(n) {}
        reference& payload() { return *nodePayload<T>(node()); }
    };

    // -------------------------------------------------------------- const_reverse_iterator
    class const_reverse_iterator
    {
        friend class var_list;
        typedef const_reverse_iterator self_t;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = int;
        using pointer = T const*;
        using reference = T const&;

        reference operator * () const noexcept { return payload(); }
        pointer operator->() const noexcept { return &payload(); }
        self_t& operator++() noexcept { toNext(); return *this; }
        self_t& operator++(int) noexcept { auto tmp = *this; toNext(); return tmp; }
        self_t& operator--() noexcept { toPrev(); return *this; }
        self_t& operator--(int) noexcept { auto tmp = *this; toPrev(); return tmp; }
        bool operator==(const self_t& o) const noexcept { return m_node == o.m_node; }
        bool operator!=(const self_t& o) const noexcept { return m_node != o.m_node; }

        bool valid() const { return m_node != nullptr; }

    protected:
        const_reverse_iterator(Node const* n) : m_node(n) {}
        Node const* m_node = nullptr;
        void toNext() { m_node = m_node->prev; }
        void toPrev() { m_node = m_node->next; }
        reference payload() const { return *nodePayload<T>(m_node); }
    };

    // -------------------------------------------------------------- reverse_iterator
    class reverse_iterator : public const_reverse_iterator
    {
        friend class var_list;
        typedef reverse_iterator self_t;
        typedef const_reverse_iterator base_t;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = int;
        using pointer = T*;
        using reference = T&;

        reference operator * () noexcept { return payload(); }
        pointer operator->() noexcept { return &payload(); }
        self_t& operator++() noexcept { base_t::toNext(); return *this; }
        self_t& operator++(int) noexcept { auto tmp = *this; base_t::toNext(); return tmp; }
        self_t& operator--() noexcept { base_t::toPrev(); return *this; }
        self_t& operator--(int) noexcept { auto tmp = *this; base_t::toPrev(); return tmp; }
        bool operator==(const self_t& o) const noexcept { return base_t::operator==(o); }
        bool operator!=(const self_t& o) const noexcept { return base_t::operator!=(o); }

    protected:
        Node* node() { return const_cast<Node*>(base_t::m_node); }
        reverse_iterator(Node* n) : const_reverse_iterator(n) {}
        reference& payload() { return *nodePayload<T>(node()); }
    };


    const_iterator begin() const { return const_iterator(m_head); }
    const_iterator end() const { return const_iterator(nullptr); }
    const_iterator last() const { return const_iterator(m_tail); }
    iterator begin() { return iterator(m_head); }
    iterator end() { return iterator(nullptr); }
    iterator last() { return iterator(m_tail); }

    const_reverse_iterator rbegin() const { return const_reverse_iterator(m_tail); }
    const_reverse_iterator rend() const { return const_reverse_iterator(nullptr); }
    const_reverse_iterator rlast() const { return const_reverse_iterator(m_head); }
    reverse_iterator rbegin() { return reverse_iterator(m_tail); }
    reverse_iterator rend() { return reverse_iterator(nullptr); }
    reverse_iterator rlast() { return reverse_iterator(m_head); }

    // todo: make it static?
    //iterator                 iter_from_ptr(T* obj)             { return iterator(nodeFromDataPtr(obj)); }
    //const_iterator           iter_from_ptr(T const* obj) const { return const_iterator(nodeFromDataPtr(obj)); }
    //reverse_iterator        riter_from_ptr(T* obj)             { return reverse_iterator(nodeFromDataPtr(obj)); }
    //const_reverse_iterator  riter_from_ptr(T const* obj) const { return const_reverse_iterator(nodeFromDataPtr(obj)); }

    static iterator                 iter_from_ptr(T* obj)       { return iterator(nodeFromDataPtr(obj)); }
    static const_iterator          citer_from_ptr(T const* obj) { return const_iterator(nodeFromDataPtr(obj)); }
    static reverse_iterator        riter_from_ptr(T* obj)       { return reverse_iterator(nodeFromDataPtr(obj)); }
    static const_reverse_iterator rciter_from_ptr(T const* obj) { return const_reverse_iterator(nodeFromDataPtr(obj)); }


    void erase(iterator i)
    {
        removeNode(i.node());
    }

    void erase(reverse_iterator i)
    {
        removeNode(i.node());
    }

    void swap(self_t& o)
    {
        std::swap(m_head, o.m_head);
        std::swap(m_tail, o.m_tail);
        std::swap(m_size, o.m_size);
    }

private:
    Node* m_head = nullptr;
    Node* m_tail = nullptr;
    size_t m_size = 0;
};

template<typename T> void swap(var_list<T>& o1, var_list<T>& o2) { o1.swap(o2); }

template<typename T> typename var_list<T>::const_iterator begin(var_list<T> const& l) { return l.begin(); }
template<typename T> typename var_list<T>::const_iterator end(var_list<T> const& l) { return l.end(); }
template<typename T> typename var_list<T>::iterator begin(var_list<T>& l) { return l.begin(); }
template<typename T> typename var_list<T>::iterator end(var_list<T>& l) { return l.end(); }

template<typename T> typename var_list<T>::const_reverse_iterator rbegin(var_list<T> const& l) { return l.begin(); }
template<typename T> typename var_list<T>::const_reverse_iterator rend(var_list<T> const& l) { return l.end(); }
template<typename T> typename var_list<T>::reverse_iterator rbegin(var_list<T>& l) { return l.begin(); }
template<typename T> typename var_list<T>::reverse_iterator rend(var_list<T>& l) { return l.end(); }


#endif // file__VAR_LIST_H

