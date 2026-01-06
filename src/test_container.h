#include <iostream>
#include <memory>

/**
 * @file test_container.h
 * @brief Тестовый контейнер для проверки аллокатора
 * 
 * @tparam T Тип элементов контейнера
 * @tparam Alloc Тип аллокатора
 */
template <typename T, typename Alloc>
class TestContainer
{
    struct TestNode
    {
    public:
        TestNode* next;
        T val;
        TestNode(const T& v) : next(nullptr), val(v) {}
    };

public:
    /**
     * @brief Итератор для обхода контейнера
     */
    class Iterator
    {
    public:
        Iterator(TestNode* node) : current(node) {}
        
        T& operator*() { return current->val; }
        const T& operator*() const { return current->val; }
        
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
        
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        
    private:
        TestNode* current;
    };

    TestContainer() : head(nullptr), back(nullptr) {}
    
    ~TestContainer() {
        // Освобождаем всю память из контейнера
        while (!empty()) {
            remove_head();
        }
    }
    
    bool empty() const { 
        return head == nullptr;
    } 

    void remove_head()
    {
        if(empty()) return;
        auto p = head;
        head = p->next;
        if (head == nullptr) {
            back = nullptr;
        }
        // Вызываем деструктор явно
        p->~TestNode();
        nodeAlloc.deallocate(p, 1);
    }

    void add(const T& val)
    {
        // Выделяем память
        auto newNode = nodeAlloc.allocate(1);
        // Используем placement new для создания объекта
        new (newNode) TestNode(val);
        
        if(empty())
        {
            back = head = newNode;
        }
        else
        {
            back->next = newNode;
            back = newNode;
        }
    }

    // Методы для итераторов (для for())
    Iterator begin() {
        return Iterator(head);
    }
    
    Iterator end() {
        return Iterator(nullptr);
    }
    
    // const версии для const контейнера
    Iterator begin() const {
        return Iterator(head);
    }
    
    Iterator end() const {
        return Iterator(nullptr);
    }

private:
    TestNode* head;
    TestNode* back;
    
    // allocator_traits для правильного rebind (см. map_allocator.h)
    using NodeAllocator = typename std::allocator_traits<Alloc>::template rebind_alloc<TestNode>;
    NodeAllocator nodeAlloc;
};