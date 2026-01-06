#include <iostream>
#include <vector>
#include <memory>

/**
 * @file map_allocator.h
 * @brief Аллокатор для std::map
 * Сделан из примера в лекции arena_allocator.cpp
 * 
 * @tparam T Тип элементов контейнера
 * @tparam N Максимальное количество элементов в контейнере
 */
template<typename T, size_t N>
class MapAllocator {
public:
    // Требуемые определения типа для аллокатора
    using value_type = T;
    
    // Статический счетчик для отслеживания выделений (общий для всех экземпляров)
    static int allocations;
    static int deallocations;
    
    MapAllocator() = default;
    
    // Конструктор копирования для разных типов (с поддержкой rebind)
    template<typename U, size_t M>
    MapAllocator(const MapAllocator<U, M>&) {}
    
    /**
     * @brief поддержка rebind для STL контейнеров
     * @tparam U Тип элементов контейнера
     */
    template<typename U>
    struct rebind {
        using other = MapAllocator<U, N>;
    };
    
    T* allocate(size_t n) {
        init_map_allocator();

        size_t bytes_needed = n * sizeof(T);

        if (map_allocator_used + bytes_needed > map_allocator_size) {
            throw std::bad_alloc();
        }


        T* result = reinterpret_cast<T*>(current);
        current += bytes_needed;

        map_allocator_used += bytes_needed;

        return result;
    }
    
    void deallocate(T* ptr, size_t n) {
        char* char_ptr = reinterpret_cast<char*>(ptr);
        if (char_ptr >= map_allocator && char_ptr < map_allocator + map_allocator_size) {
            // std::cout << "Ignoring deallocate request for map_allocator memory\n";
        } else {
            // std::cout << "Freeing non-map_allocator memory\n";
            std::free(ptr);
        }
    }
    
    // Print statistics
    static void printStats() {
        std::cout << "Total allocations: " << allocations << "\n";
        std::cout << "Total deallocations: " << deallocations << "\n";
    }
private:
    static char* map_allocator;
    static char* current;
    static size_t map_allocator_used;
    static size_t map_allocator_size;
    static bool map_allocator_initialized;
    
    // Initialize map_allocator
    static void init_map_allocator() {
        if (map_allocator_initialized) return;
        map_allocator_size = sizeof(T) * N;
        map_allocator = static_cast<char*>(std::malloc(map_allocator_size));
        current = map_allocator;
        map_allocator_used = 0;
        map_allocator_initialized = true;
    }
};

// ------------------------------------------------------------------------------------------------
// Инициализация статических членов 
// ------------------------------------------------------------------------------------------------

template<typename T, size_t N>
char* MapAllocator<T, N>::map_allocator = nullptr;

template<typename T, size_t N>
char* MapAllocator<T, N>::current = nullptr;

template<typename T, size_t N>
size_t MapAllocator<T, N>::map_allocator_size = 0;

template<typename T, size_t N>
size_t MapAllocator<T, N>::map_allocator_used = 0;

template<typename T, size_t N>
bool MapAllocator<T, N>::map_allocator_initialized = false;

template<typename T, size_t N, typename U, size_t M>
bool operator==(const MapAllocator<T, N>&, const MapAllocator<U, M>&) {
    return true;
}

template<typename T, size_t N, typename U, size_t M>
bool operator!=(const MapAllocator<T, N>&, const MapAllocator<U, M>&) {
    return false;
}