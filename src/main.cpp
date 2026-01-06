#include "lib.h"
#include <iostream>
#include <map>
#include <memory>
#include "map_allocator.h"
#include "test_container.h"

// Функция для вычисления факториала
int factorial(int n) {
    if (n <= 1) return 1;
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main()
{
    // 1. создание экземпляра std::map<int, int>
    std::map<int, int> standardMap;
    
    // 2. заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    for (int i = 0; i < 10; ++i) {
        standardMap[i] = factorial(i);
    }
    
    // 3. создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
    // std::map использует std::pair<const Key, Value>, поэтому аллокатор должен быть для pair (task.md)
    using MapValueType = std::pair<const int, int>;
    std::map<int, int, std::less<int>, MapAllocator<MapValueType, 10>> customAllocatorMap;
    
    // 4. заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    for (int i = 0; i < 10; ++i) {
        customAllocatorMap[i] = factorial(i);
    }
    
    // 5. вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
    std::cout << "Standard map contents:\n";
    for (const auto& pair : standardMap) {
        std::cout << pair.first << " " << pair.second << "\n";
    }
    
    std::cout << "\nCustom allocator map contents:\n";
    for (const auto& pair : customAllocatorMap) {
        std::cout << pair.first << " " << pair.second << "\n";
    }
    
    // 6. создание экземпляра своего контейнера для хранения значений типа int
    TestContainer<int, std::allocator<int>> standardContainer;
    // 7. заполнение 10 элементами от 0 до 9
    for (int i = 0; i < 10; ++i) {
        standardContainer.add(i);
    }
    
    // 8. создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором, ограниченным 10 элементами
    TestContainer<int, MapAllocator<int, 10>> customAllocatorContainer;
    // 9. заполнение 10 элементами от 0 до 9
    for (int i = 0; i < 10; ++i) {
        customAllocatorContainer.add(i);
    }
    
    // 10. вывод на экран всех значений, хранящихся в контейнере
    std::cout << "\nStandard allocator container contents:\n";
    for (const auto& val : standardContainer) {
        std::cout << val << " ";
    }
    std::cout << "\n";
    
    std::cout << "\nCustom allocator container contents:\n";
    for (const auto& val : customAllocatorContainer) {
        std::cout << val << " ";
    }
    std::cout << "\n";
    return 0;
}