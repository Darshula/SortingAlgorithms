#include <algorithm>
#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>

constexpr double nanoToMicro{0.001};
constexpr double microToMilli{0.001};

class Timer {
public:
  Timer() : m_StartTimepoint(std::chrono::high_resolution_clock::now()){};
  Timer(const Timer &) = delete;            // copy constructor
  Timer &operator=(const Timer &) = delete; // copy assignment operator
  Timer(Timer &&) = delete;                 // move constructor
  Timer &operator=(Timer &&) = delete;      // move operator
  ~Timer()
  {
    auto endTimepoint{std::chrono::high_resolution_clock::now()};
    auto start{
        std::chrono::time_point_cast<std::chrono::nanoseconds>(m_StartTimepoint)
            .time_since_epoch()
            .count()};
    auto end{
        std::chrono::time_point_cast<std::chrono::nanoseconds>(endTimepoint)
            .time_since_epoch()
            .count()};
    double duration{static_cast<double>(end - start) * nanoToMicro *
                    microToMilli};
    std::cout << std::setprecision(std::numeric_limits<double>::max_digits10)
              << "[TIMER]: " << duration << "ms" << '\n';
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};

template <uint64_t SIZE> void randomizeArray(std::array<int32_t, SIZE> &array)
{
  std::mt19937 MersenneTwister{static_cast<uint32_t>(
      std::chrono::high_resolution_clock::now().time_since_epoch().count())};
  std::uniform_int_distribution<int32_t> generate{INT8_MIN, INT8_MAX + 1};

  for (uint64_t i{}; i < SIZE; ++i) {
    array.at(i) = generate(MersenneTwister);
  }
}

template <uint64_t SIZE> void bubbleSort(std::array<int32_t, SIZE> &array)
{
  uint64_t unsortedSize{array.size()};
  uint64_t newSize{};
  bool swapped{};
  do {
    swapped = false;
    for (uint32_t i{1}; i < unsortedSize; ++i) {
      if (array.at(i - 1) > array.at(i)) {
        std::swap(array.at(i - 1), array.at(i));
        swapped = true;
        newSize = i;
      }
    }
    unsortedSize = newSize;
  } while (swapped);
}

template <uint64_t SIZE> void insertionSort(std::array<int32_t, SIZE> &array)
{
  uint64_t currIndex{1};
  uint64_t swapIndex{};
  while (currIndex < array.size()) {
    swapIndex = currIndex;
    while (swapIndex > 0 && array.at(swapIndex - 1) > array.at(swapIndex)) {
      std::swap(array.at(swapIndex - 1), array.at(swapIndex));
      --swapIndex;
    }
    ++currIndex;
  }
}

template <uint64_t SIZE> void selectionSort(std::array<int32_t, SIZE> &array)
{
  uint64_t min{};
  for (uint64_t i{}; i < array.size() - 1; ++i) {
    min = i;
    for (uint64_t j{i + 1}; j < array.size(); ++j) {
      if (array.at(j) < array.at(min)) {
        min = j;
      }
    }
    std::swap(array.at(min), array.at(i));
  }
}

template <uint64_t SIZE>
uint64_t partition(std::array<int32_t, SIZE> &array, const uint64_t &low,
                   const uint64_t &high)
{
  int64_t pivot{array.at(low + ((high - low) / 2))};
  uint64_t rightIndex{low - 1};
  uint64_t leftIndex{high + 1};
  while (true) {
    do {
      ++rightIndex;
    } while (array.at(rightIndex) < pivot);
    do {
      --leftIndex;
    } while (pivot < array.at(leftIndex));
    if (!(rightIndex < leftIndex)) {
      return leftIndex;
    }
    std::swap(array.at(rightIndex), array.at(leftIndex));
  }
}

template <uint64_t SIZE>
void quickSort(std::array<int32_t, SIZE> &array, const uint64_t &low,
               const uint64_t &high)
{
  if (low < high) {
    uint64_t part{partition(array, low, high)};
    quickSort(array, low, part);
    quickSort(array, part + 1, high);
  }
}

template <uint64_t SIZE> void quickSort(std::array<int32_t, SIZE> &array)
{
  quickSort(array, 0, array.size() - 1);
}

template <uint64_t SIZE>
void merge(std::array<int32_t, SIZE> &array, const uint64_t &low,
           const uint64_t &mid, const uint64_t &high)
{
  const uint64_t leftArraySize{mid - low + 1};
  const uint64_t rightArraySize{high - mid};
  uint64_t leftArrayIndex{};
  uint64_t rightArrayIndex{};
  uint64_t mergedArrayIndex{low};
  std::unique_ptr<int32_t[]> leftArray{new int32_t[leftArraySize]};
  std::unique_ptr<int32_t[]> rightArray{new int32_t[rightArraySize]};
  for (uint64_t leftIndex{}; leftIndex < leftArraySize; ++leftIndex) {
    leftArray[leftIndex] = array.at(low + leftIndex);
  }
  for (uint64_t rightIndex{}; rightIndex < rightArraySize; ++rightIndex) {
    rightArray[rightIndex] = array.at(mid + rightIndex + 1);
  }
  while (leftArrayIndex < leftArraySize && rightArrayIndex < rightArraySize) {
    if (leftArray[leftArrayIndex] < rightArray[rightArrayIndex]) {
      array.at(mergedArrayIndex) = leftArray[leftArrayIndex];
      ++leftArrayIndex;
    }
    else {
      array.at(mergedArrayIndex) = rightArray[rightArrayIndex];
      ++rightArrayIndex;
    }
    ++mergedArrayIndex;
  }
  while (leftArrayIndex < leftArraySize) {
    array.at(mergedArrayIndex) = leftArray[leftArrayIndex];
    ++leftArrayIndex;
    ++mergedArrayIndex;
  }
  while (rightArrayIndex < rightArraySize) {
    array.at(mergedArrayIndex) = rightArray[rightArrayIndex];
    ++rightArrayIndex;
    ++mergedArrayIndex;
  }
}

template <uint64_t SIZE>
void mergeSort(std::array<int32_t, SIZE> &array, const uint64_t &low,
               const uint64_t &high)
{
  if (high - low > 1) {
    const uint64_t mid = (low + (high - low) / 2);
    mergeSort(array, low, mid);
    mergeSort(array, mid + 1, high);
    merge(array, low, mid, high);
  }
  else {
    if (array.at(high) < array.at(low)) {
      std::swap(array.at(high), array.at(low));
    }
  }
}

template <uint64_t SIZE> void mergeSort(std::array<int32_t, SIZE> &array)
{
  mergeSort(array, 0, array.size() - 1);
}

template <uint64_t SIZE> void radixSort(std::array<int32_t, SIZE> &array)
{
  ++array.at(0);
}

template <uint64_t SIZE>
void heapify(std::array<int32_t, SIZE> &array, const uint64_t &size,
             const uint64_t &index)
{
  uint64_t largest{index};
  const uint64_t left{2 * index + 1};
  const uint64_t right{left + 1};
  if (left < size && array.at(left) > array.at(largest)) {
    largest = left;
  }
  if (right < size && array.at(right) > array.at(largest)) {
    largest = right;
  }
  if (largest != index) {
    std::swap(array.at(index), array.at(largest));
    heapify(array, size, largest);
  }
}

template <uint64_t SIZE> void heapSort(std::array<int32_t, SIZE> &array)
{
  for (uint64_t index{array.size() / 2 - 1}; index != 0; --index) {
    heapify(array, array.size(), index);
  }
  heapify(array, array.size(), 0);
  for (uint64_t i{array.size() - 1}; i > 0; --i) {
    std::swap(array.at(0), array.at(i));
    heapify(array, i, 0);
  }
}

template <uint64_t SIZE> void printArray(std::array<int32_t, SIZE> &array)
{
  for (int32_t &element : array) {
    std::cout << element << ' ';
  }
}

int32_t main()
{
  enum options {
    None = '0',
    Bubble,
    Insertion,
    Selection,
    Quick,
    Merge,
    Radix,
    Heap
  };
  std::array<int32_t, INT16_MAX> array{};
  char input{};
  bool quit{};
  while (!quit) {
    randomizeArray(array);
    std::cin >> input;
    Timer timer;
    switch (input) {
    case None:
      std::cout << "Original array:" << '\n';
      for (int64_t value : array) {
        std::cout << value << ' ';
      }
      std::cout << '\n';
      break;
    case Bubble:
      std::cout << "Bubble Sort ";
      bubbleSort(array);
      break;
    case Insertion:
      std::cout << "Insertion Sort ";
      insertionSort(array);
      break;
    case Selection:
      std::cout << "Selection Sort ";
      selectionSort(array);
      break;
    case Quick:
      std::cout << "Quick Sort ";
      quickSort(array);
      break;
    case Merge:
      std::cout << "Merge Sort ";
      mergeSort(array);
      break;
    case Radix:
      std::cout << "Radix Sort ";
      radixSort(array);
      break;
    case Heap:
      std::cout << "Heap Sort ";
      heapSort(array);
      break;
    default:
      quit ^= 1;
    }
    std::cout << "[RESULT]: ";
    if (std::is_sorted(array.begin(), array.end())) {
      std::cout << "successful";
    }
    else {
      std::cout << "unsuccessful";
    }
    std::cout << std::endl;
    // printArray(array);
  }
  return 0;
}
