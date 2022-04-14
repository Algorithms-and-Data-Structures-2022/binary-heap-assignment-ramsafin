#include "assignment/min_binary_heap.hpp"

#include <algorithm>  // fill
#include <stdexcept>  // invalid_argument
#include <limits>     // numeric_limits

namespace assignment {

  MinBinaryHeap::MinBinaryHeap(int capacity) {

    if (capacity <= 0) {
      throw std::invalid_argument("capacity must be positive");
    }

    // инициализируем поля
    size_ = 0;
    capacity_ = capacity;

    // выделяем память для узлов
    data_ = new Node[capacity_];

    // заполняем массив "пустыми узлами"
    std::fill(data_, data_ + capacity_, Node{});
  }

  MinBinaryHeap::~MinBinaryHeap() {

    // обнуляем поля
    size_ = 0;
    capacity_ = 0;

    // высвобождаем выделенную память
    delete[] data_;
    data_ = nullptr;
  }

  bool MinBinaryHeap::Insert(int key, int value) {

    if (size_ == capacity_) {
      // двоичная куча заполнена, операция вставки нового узла невозможна
      return false;
    }

    const int insert_index = size_;

    data_[insert_index] = Node(key, value);
    size_ += 1;

    sift_up(insert_index);

    return true;
  }

  std::optional<int> MinBinaryHeap::Extract() {

    if (size_ == 0) {
      // двоичная куча пустая, операция извлечения корня невозможна
      return std::nullopt;
    }

    const int extracted = data_[0].value;

    data_[0] = data_[size_ - 1];
    size_--;

    heapify(0);

    return extracted;
  }

  bool MinBinaryHeap::Remove(int key) {

    constexpr int min_key_value = std::numeric_limits<int>::min();

    const auto remove_index = search_index(key);

    if (!remove_index.has_value()) {
      return false;
    }

    data_[remove_index.value()].key = min_key_value;
    sift_up(remove_index.value());

    Extract();

    return true;
  }

  void MinBinaryHeap::Clear() {
    size_ = 0;
  }

  std::optional<int> MinBinaryHeap::Search(int key) const {

    const auto index = search_index(key);

    if (index.has_value()) {
      return data_[index.value()].value;
    }

    return std::nullopt;
  }

  bool MinBinaryHeap::Contains(int key) const {
    return Search(key).has_value();
  }

  bool MinBinaryHeap::IsEmpty() const {
    return size_ == 0;
  }

  int MinBinaryHeap::capacity() const {
    return capacity_;
  }

  int MinBinaryHeap::size() const {
    return size_;
  }

  void MinBinaryHeap::sift_up(int index) {

    // Алгоритм:
    // Пока index не равен индексу корневого узла И ключ узла меньше ключа родителя:
    //  поднимаем "наверх" узел - меняем местами нижний и верхний узлы (swap)
    //  index = индекс родительского узла

    while (index != 0 && data_[index].key < data_[parent_index(index)].key) {

      std::swap(data_[index], data_[parent_index(index)]);
      index = parent_index(index);
    }
  }

  void MinBinaryHeap::heapify(int index) {

    // индексы левого и правого потомков узла с индексом index
    const int left_index = left_child_index(index);
    const int right_index = right_child_index(index);

    // вышли за пределы массива, останавливаемся
    if (left_index >= size_) {
      return;
    }

    // индекс узла-потомка с наименьшим значением ключа
    int smallest_key_index = index;

    if (data_[left_index].key < data_[index].key) {
      smallest_key_index = left_index;
    }

    if (data_[right_index].key < data_[smallest_key_index].key) {
      smallest_key_index = right_index;
    }

    // если индекс наименьшего узла найден (не равен индексу самого узла)
    if (smallest_key_index != index) {

      // меняем местами родителя и потомка (swap)
      std::swap(data_[index], data_[smallest_key_index]);

      // рекурсивно спускаемся по куче, следуя индексу
      heapify(smallest_key_index);
    }
  }

  std::optional<int> MinBinaryHeap::search_index(int key) const {

    for (int index = 0; index < size_; ++index) {
      if (data_[index].key == key) {
        return index;
      }
    }

    return std::nullopt;
  }

}  // namespace assignment
