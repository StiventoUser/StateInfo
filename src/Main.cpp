#include <exception>
#include <string>
#include <type_traits>

#include <iostream>

#include "StateException.h"

struct Info
{
	std::string Name;
	int Version;
	float Precision;
	bool IsActive;
};

template<>
class StateInfo::ClassToString<Info>
{
public:
	static std::string GetClassName()
	{
		return "Info";
	}

	static std::string GetValue(const Info& value)
	{
		std::string resStr;

		resStr.STATEEXCEPT_FORMATVAR(value.Name).append(", ")
			.STATEEXCEPT_FORMATVAR(value.Version).append(", ")
			.STATEEXCEPT_FORMATVAR(value.Precision).append(", ")
			.STATEEXCEPT_FORMATVAR(value.IsActive);

		return resStr;
	}
};

void foo1()
{
	const int i1 = 50;
	const int& i2 = i1;
	const bool& b1 = true;
	bool b2 = true;
	bool* b3 = &b2;
	char ch = 'g';
	const char* str = "qwerty";

	throw STATEEXCEPT_NEW("foo1 is failed")
		.STATEEXCEPT_USE(i1)
		.STATEEXCEPT_USE(b1)
		.STATEEXCEPT_USE(i2)
		.STATEEXCEPT_USE(b3)
		.STATEEXCEPT_USE(ch)
		.STATEEXCEPT_USE(str)
		.LockState();
}

void foo2()
{
	std::string str = "foo2 string var";
	float floatAcc = 9.8f;
	double maxVel = 500.015;
	char* pVal = nullptr;

	try
	{
		foo1();
	}
	catch (const std::exception& e)
	{
		throw STATEEXCEPT_WRAP("foo2 is failed", e)
			.STATEEXCEPT_USE(str)
			.STATEEXCEPT_USE(floatAcc)
			.STATEEXCEPT_USE(maxVel)
			.STATEEXCEPT_USE(pVal)
			.LockState();
	}
}

void foo3()
{
	Info info { "AbcTest", 128, 0.02f, true };
	const Info& info2 = info;
	const Info* info3 = &info;

	try
	{
		foo2();
	}
	catch (const std::exception& e)
	{
		throw STATEEXCEPT_WRAP("foo3 is failed", e)
			.STATEEXCEPT_USE(info2)
			.STATEEXCEPT_USE(info3)
			.LockState();
	}
}

int main()
{
	try
	{
		foo3();
	}
	catch (StateInfo::StateException& e)
	{
		std::cout << "Full  message: " << e.what() /*or GetFullMessage()*/ << std::endl << std::endl;
		std::cout << "Short message: " << e.GetLastMessage() << std::endl;

		/*
		Output:

		Full  message: Full  message: "foo3 is failed" at C:\Development\Projects\StateInfo\src\Main.cpp:93 with variables: 
		[info2 = (Info) : {value.Name = (std::string) : {AbcTest}, value.Version = 128, value.Precision = 0.020000, value.IsActive = true}, 
		info3 = (from pointer) (Info) : {value.Name = (std::string) : {AbcTest}, value.Version = 128, value.Precision = 0.020000, value.IsActive = true}] 
			which is caused by:
			"foo2 is failed" at C:\Development\Projects\StateInfo\src\Main.cpp:72 with variables:
			[str = (std::string) : {foo2 string var}, floatAcc = 9.800000, maxVel = 500.015000, pVal = null] 
				which is caused by:
				"foo1 is failed" at C:\Development\Projects\StateInfo\src\Main.cpp:49 with variables:
				[i1 = 50, b1 = true, i2 = 50, b3 = (from pointer) true, ch = 103, str = (from pointer) qwerty]

		Short message: foo3 is failed
		*/
	}
}
