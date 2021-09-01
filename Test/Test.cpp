#include <iostream>

template<class T>
class TArray
{
	friend class FString;

private:
	T* data;
	int32_t numElements;
	int32_t maxElements;

public:
	inline int32_t Num()
	{
		return numElements;
	}
	inline bool IsValidIndex(int32_t index)
	{
		return index >= 0 && index < numElements;
	}
	inline T& operator[](int32_t index)
	{
		return data[index];
	}
};

class FString : TArray<wchar_t>
{
public:
	FString(const wchar_t* other)
	{
		maxElements = numElements = *other ? std::wcslen(other) + 1 : 0;

		if (numElements)
		{
			data = const_cast<wchar_t*>(other);
		}
	};

	inline std::string ToString()
	{
		std::wstring wStr(data);
		return std::string(wStr.begin(), wStr.end()).erase(wStr.length() - 1, 1);
	}
};

int main()
{
	FString myFStr(L"Hello World!");
}
