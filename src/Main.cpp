#include <exception>
#include <string>
#include <vector>
#include <type_traits>

#include <iostream>

#include "StateException.h"

class Info
{
	friend class StateInfo::ClassToString<Info>;

public:
	Info(const std::string& n, const int v, const float p, const bool a)
		: Name(n), Version(v), Precision(p), IsActive(a)
	{
	}

private:
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

	static std::string GetValue(const Info& obj)
	{
		auto resStr = STATEEXCEPT_FORMATCLASS(obj, (Name)(Version)(Precision)(IsActive));

		return resStr;
	}
};

void foo0()
{
	throw STATEEXCEPT_NEW("new error");
}

void foo1()
{
	try
	{
		foo0();
	}
	catch (const std::exception& e)
	{
		throw STATEEXCEPT_WRAP("foo1 is failed", e);
	}
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
	STATEEXCEPT_CATCH_AND_RETHROW_DEBUG(std::exception, "foo2 is failed", (str)(maxVel)(pVal))
}

void foo3()
{
	std::vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	std::vector<Info> vec2 = { {"a", 1, 0.1f, true}, { "b", 2, 0.2f, false },
		{ "c", 3, 0.3f, true }, { "d", 4, 0.4f, false } };

	try
	{
		foo2();
	}
	STATEEXCEPT_CATCH_AND_RETHROW(std::exception, "foo3 is failed", (vec)(vec2));
}

void foo4()
{
	const int i1 = 50;
	const int& i2 = i1;
	const bool& b1 = true;
	bool b2 = true;
	bool* b3 = &b2;
	char ch = 'g';
	const char* str = "qwerty";
	char str2[20] = "asdfgh";

	try
	{
		foo3();
	}
	STATEEXCEPT_CATCH_AND_RETHROW_DEBUG(std::exception, "foo4 is failed", (i1)(b1)(b3)(str)(str2))
}

void foo5()
{
	Info info { "AbcTest", 128, 0.02f, true };
	const Info& info2 = info;
	const Info* info3 = &info;

	try
	{
		foo4();
	}
	catch (const std::exception& e)
	{
		// important code.

		STATEEXCEPT_WRAP_AND_RETHROW("foo5 is failed", e, (info3));
	}
}

int main()
{
	try
	{
		foo5();
	}
	catch (StateInfo::StateException& e)
	{
		std::cout << "Full  message: " << e.what() /*or GetFullMessage()*/ << std::endl << std::endl;
		std::cout << "Short message: " << e.GetLastMessage() << std::endl;

		/*
		Output:

		Full  message: "foo5 is failed" at C:\Development\Projects\StateInfo\src\Main.cpp:120 with variables: [
			info3 = (from pointer) (Info) : {Name = (std::string) : {AbcTest};Version = 128;Precision = 0.020000;IsActive = true;} ] which is caused by:
		"foo4 is failed" at C:\Development\Projects\StateInfo\src\Main.cpp:103 with variables: [
			i1 = 50,
			b1 = true,
			b3 = (from pointer) true,
			str = (from pointer) qwerty,
			str2 = (from pointer) asdfgh ] which is caused by:
		"foo3 is failed" at C:\Development\Projects\StateInfo\src\Main.cpp:85 with variables: [
			vec = (std::vector<T>) : {[1, 2, 3, 4, 5, 6, 7, 8, 9, 10, ...]},
			vec2 = (std::vector<T>) : {[(Info) : {Name = (std::string) : {a};Version = 1;Precision = 0.100000;IsActive = true;}, (Info) : {Name = (std::string) : {b};Version = 2;Precision = 0.200000;IsActive = false;}, (Info) : {Name = (std::string) : {c};Version = 3;Precision = 0.300000;IsActive = true;}, (Info) : {Name = (std::string) : {d};Version = 4;Precision = 0.400000;IsActive = false;}]} ] which is caused by:
		"foo2 is failed" at C:\Development\Projects\StateInfo\src\Main.cpp:72 with variables: [
			str = (std::string) : {foo2 string var},
			maxVel = 500.015000,
			pVal = null ] which is caused by:
		"foo1 is failed" at C:\Development\Projects\StateInfo\src\Main.cpp:57 which is caused by:
		"new error" at C:\Development\Projects\StateInfo\src\Main.cpp:46

		Short message: foo5 is failed
		*/
	}
	catch (const std::exception& e)
	{
		std::cout << "Message: " << e.what() << std::endl;
	}
}
