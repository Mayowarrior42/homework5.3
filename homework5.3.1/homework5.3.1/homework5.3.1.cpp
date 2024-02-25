#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <stdexcept>
#include <iostream>

struct listnode {
public:
    listnode(int value, listnode* prev = nullptr, listnode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    listnode* prev;
    listnode* next;
};


class list {
public:
    list()
        : m_head(new listnode(static_cast<int>(0))), m_size(0),
        m_tail(new listnode(0, m_head))
    {
    }

    virtual ~list()
    {
        clear();
        delete m_head;
        delete m_tail;
    }

    bool empty() { return m_size == 0; }

    unsigned long size() { return m_size; }

    void pushfront(int value)
    {
        new listnode(value, m_head, m_head->next);
        ++m_size;
    }

    void pushback(int value)
    {
        new listnode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int popfront()
    {
        if (empty()) throw std::runtime_error("list is empty");
        auto node = extractprev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int popback()
    {
        if (empty()) throw std::runtime_error("list is empty");
        auto node = extractprev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractprev(current);
        }
    }

private:
    listnode* extractprev(listnode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    listnode* m_head;
    listnode* m_tail;
    unsigned long m_size;
};

TEST_CASE("Testing the List functions") {
    list myList;

    SECTION("Testing the empty function on a newly created list") {
        REQUIRE(myList.empty() == true);
    }

    SECTION("Testing the size function on an empty list") {
        REQUIRE(myList.size() == 0);
    }

    SECTION("Testing the size function after adding elements") {
        myList.pushback(1);
        myList.pushback(2);
        myList.pushback(3);
        REQUIRE(myList.size() == 3);
    }

    SECTION("Testing the clear function") {
        myList.pushback(1);
        myList.pushback(2);
        myList.pushback(3);
        myList.clear();
        REQUIRE(myList.empty() == true);
        REQUIRE(myList.size() == 0);
    }
}
