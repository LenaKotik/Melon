#include "Melon.hpp"


template <typename T>
inline Melon::DynamicArray<T>::DynamicArray(size_t initial_size) : capacity(initial_size), size(0), step(1)
{
	data = new T[capacity];
}

template <typename T>
size_t Melon::DynamicArray<T>::Size()
{
	return size;
}

template <typename T>
T* Melon::DynamicArray<T>::Data()
{
	return data;
}

template <typename T>
void Melon::DynamicArray<T>::Shrink()
{
	delete[](T[capacity])data;
	capacity = size;
	data = new T[capacity];
}

template <typename T>
void Melon::DynamicArray<T>::PushBack(T&& value)
{
	data[size] = value;
	size++;
	if (capacity < size)
	{
		capacity += step;
		T* dataTemp = new T[capacity];
		delete[](T[capacity])data;
	}
}
//				[3][0][0][0][0]
//				    ^ size   
//				             ^ capacity
template<typename T>
T Melon::DynamicArray<T>::PopBack()
{
	size--;
	return data[size];
}

template<typename T>
inline Melon::DynamicArray<T>::~DynamicArray()
{
}
