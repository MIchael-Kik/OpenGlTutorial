#pragma once


template <typename T>
class Array
{
public:
	Array(); 
	~Array();

	const unsigned int size() const;

	void push(T input);
    void pop();

	const T& operator [] (unsigned int index) const;
	T& operator [](unsigned int index);
private:
	T* m_Buffer = nullptr;

	unsigned int m_MemAlloc = 5;
	unsigned int m_MemUsed = 0;

};
