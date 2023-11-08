#pragma once

template<typename T>
class dynamicArr
{
private:
	T** data;

	unsigned int countElements;

	T** AllocateMem(T** arr, const unsigned int offset);
	T** Swap(T** prevMat, T** newMat);
	void CleanUpArray();
public:
	dynamicArr();

	void Push(T dat);
	void Pop(const int& elem);
	void Clear();

	T* at(const unsigned int index);
	unsigned int GetCountElements() { return countElements; }

	T* operator[](const unsigned int index);

	~dynamicArr();
};

template<typename T>
inline T** dynamicArr<T>::AllocateMem(T** arr, const unsigned int offset)
{
	arr = new T * [countElements + offset] {nullptr};
	return arr;
}

template<typename T>
inline T** dynamicArr<T>::Swap(T** prevMat, T** newMat)
{
	for (unsigned int i = 0; i < countElements; i++)
		if(prevMat[i] != nullptr)
			newMat[i] = prevMat[i];
	return newMat;
}

template<typename T>
inline void dynamicArr<T>::CleanUpArray()
{
	if (this->data)
	{
		delete[] this->data;

		this->data = nullptr;
	}
}

template<typename T>
inline dynamicArr<T>::dynamicArr()
{
	data = nullptr;
	countElements = 0;
}

template<typename T>
inline void dynamicArr<T>::Push(T dat)
{
		T** newArr = nullptr;
		newArr = AllocateMem(newArr, countElements + 1);

		newArr = Swap(data, newArr);
		newArr[countElements] = new T{ dat };

		CleanUpArray(); countElements++;
		data = newArr;
}

template<typename T>
inline void dynamicArr<T>::Pop(const int& elem)
{
	for (unsigned int i = 0; i < countElements; i++)
	{
		if (i + 1 == elem)
		{
			delete data[i];
			data[i] = nullptr;
			T** newArr = nullptr;

			newArr = AllocateMem(newArr, countElements - 1);
			newArr = Swap(data, newArr);
			CleanUpArray(); countElements--;
			data = newArr;
			break;
		}
	}
}

template<typename T>
inline void dynamicArr<T>::Clear()
{
	CleanUpArray();
	countElements = 0;
	data = nullptr;
}

template<typename T>
inline T* dynamicArr<T>::at(const unsigned int index)
{
	if (index < 0 || index >= countElements)
		throw("Invalid index");
	else if (data == nullptr)
		throw("Matrix is empty");

	return data[index];
}

template<typename T>
inline T* dynamicArr<T>::operator[](const unsigned int index)
{
	if (index < 0 || index >= countElements)
		throw("Invalid index");
	else if (data == nullptr)
		throw("Matrix is empty");

	return data[index];
}

template<typename T>
inline dynamicArr<T>::~dynamicArr()
{
	Clear();
}
