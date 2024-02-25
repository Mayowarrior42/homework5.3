#define CATCH_CONFIG_MAIN
#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <stdexcept>



struct ListNode
{
public:
    ListNode(int value, ListNode* prev = nullptr, ListNode* next = nullptr)
        : value(value), prev(prev), next(next)
    {
        if (prev != nullptr) prev->next = this;
        if (next != nullptr) next->prev = this;
    }

public:
    int value;
    ListNode* prev;
    ListNode* next;
};


class List
{
public:
    List()
        : m_head(new ListNode(static_cast<int>(0))), m_size(0),
        m_tail(new ListNode(0, m_head))
    {
    }

    virtual ~List()
    {
        Clear();
        delete m_head;
        delete m_tail;
    }

    bool Empty() { return m_size == 0; }

    unsigned long Size() { return m_size; }

    void PushFront(int value)
    {
        new ListNode(value, m_head, m_head->next);
        ++m_size;
    }

    void PushBack(int value)
    {
        new ListNode(value, m_tail->prev, m_tail);
        ++m_size;
    }

    int PopFront()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_head->next->next);
        int ret = node->value;
        delete node;
        return ret;
    }

    int PopBack()
    {
        if (Empty()) throw std::runtime_error("list is empty");
        auto node = extractPrev(m_tail);
        int ret = node->value;
        delete node;
        return ret;
    }

    void Clear()
    {
        auto current = m_head->next;
        while (current != m_tail)
        {
            current = current->next;
            delete extractPrev(current);
        }
    }

private:
    ListNode* extractPrev(ListNode* node)
    {
        auto target = node->prev;
        target->prev->next = target->next;
        target->next->prev = target->prev;
        --m_size;
        return target;
    }

private:
    ListNode* m_head;
    ListNode* m_tail;
    unsigned long m_size;
};


TEST_CASE("Testing List functionality") {
    SECTION("PushBack and PushFront") {
        List list;

        REQUIRE(list.Empty());
        REQUIRE(list.Size() == 0);

        list.PushFront(5);
        REQUIRE_FALSE(list.Empty());
        REQUIRE(list.Size() == 1);

        list.PushBack(10);
        REQUIRE(list.Size() == 2);


        REQUIRE(list.PopFront() == 5);
        REQUIRE(list.PopFront() == 10);
    }

    SECTION("PopFront and PopBack on empty list") {
        List list;

        REQUIRE(list.Empty());


        REQUIRE_THROWS_AS(list.PopFront(), std::runtime_error);
        REQUIRE_THROWS_AS(list.PopBack(), std::runtime_error);
    }

    SECTION("Multiple operations") {
        List list;

        list.PushFront(5);
        list.PushFront(10);
        list.PushBack(15);

        REQUIRE(list.Size() == 3);

  
        REQUIRE(list.PopFront() == 10);
        REQUIRE(list.PopFront() == 5);
        REQUIRE(list.PopFront() == 15);
    }

    SECTION("Complex scenario") {
        List list;

        list.PushFront(5);
        list.PushBack(10);
        list.PushFront(3);

        REQUIRE(list.Size() == 3);

      
        REQUIRE(list.PopFront() == 3);
        REQUIRE(list.PopBack() == 10);
        REQUIRE(list.PopFront() == 5);

     
        REQUIRE(list.Empty());
        REQUIRE(list.Size() == 0);
    }
}