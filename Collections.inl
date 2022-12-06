#include "Melon.hpp"
template<typename T>
inline Size_t Melon::DynamicArray<T>::Size() const
{
	return size;
}
template<typename T>
inline Size_t Melon::DynamicArray<T>::ByteSize() const
{
	return actual_size * sizeof(T);
}
template<typename T>
inline bool Melon::DynamicArray<T>::Resize(Size_t end_size)
{
	end_size = min(end_size,size); // no data loss;
	if (end_size == actual_size) return true; // we didn't fail if we did nothing, my motto in life
	actual_size = end_size;
	T* temp = Data;
	Data = new T[actual_size];
	if (!Data) return false;
	std::memcpy(Data, temp, size * sizeof(T));
	delete temp;
	return true;
}
template<typename T>
inline void Melon::DynamicArray<T>::PushBack(T element)
{
	if (actual_size - size >= 1) // base case
	{
		Data[size++] = element;
		return;
	}
	T* temp = Data;
	Data = new T[++actual_size];
	std::memcpy(Data, temp, size * sizeof(T));
	delete temp;
	return PushBack(element); // uneccessery recursion
}
template<typename T>
inline T Melon::DynamicArray<T>::PeekBack()
{
	if (size < 1) return T();
	return Data[size];
}
template<typename T>
inline T Melon::DynamicArray<T>::PopBack()
{
	if (size < 1) return T();
	return Data[--size];
}
template<typename T>
inline T& Melon::DynamicArray<T>::operator[](Size_t index)
{
	return Data[index];
}
template<typename T>
inline const T& Melon::DynamicArray<T>::operator[](Size_t index) const
{
	return Data[index];
}
template<typename T>
inline T* Melon::DynamicArray<T>::begin() const
{
	return Data;
}
template<typename T>
inline T* Melon::DynamicArray<T>::end() const
{
	return Data+size;
}
template <typename T>
inline Melon::DynamicArray<T>::DynamicArray(std::initializer_list<T> list)
{
	actual_size = list.size();
	size = actual_size;
	Data = new T[size];
	std::memcpy(Data, list.begin(), actual_size*sizeof(T));
}

template<typename T>
inline Melon::DynamicArray<T>::DynamicArray()
{
	Data = new T[1];
	size = 0;
	actual_size = 1;
}
