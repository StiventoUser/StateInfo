#pragma once

namespace StateInfo
{

namespace Internal
{

//! Max number of elements in a sequence that displayed in exception.
constexpr const size_t MaxSequenceDisplayLength = 10;

} // namespace Internal

//! An alias of type that exists if 2 types are same (with ignored qualifiers)
template<typename T, typename ExpectedT>
using EnableIfSame_T = std::enable_if_t<std::is_same<std::decay_t<T>, std::decay_t<ExpectedT>>::value>;

//
// Declaration of the TypeToString class.
// It's used to produce an error for unknown type.
//

template<typename T, typename = void>
class TypeToString
{
};

//
// Declaration of the ClassToString class. 
// This class is used to produce an error for unknown class.
// Specialize it for your class to make it TypeToString compatible.
// You have to define these functions:
// static std::string GetClassName() - that returns class name.
// static std::string GetValue(const YourClass& value) - that returns class state as string.
//

template<typename T>
class ClassToString
{
};

//
// Declaration of the TypeToString class.
// It's a specialization for the integer and floating point types.
//

template<typename T>
class TypeToString<T, std::enable_if_t<std::is_arithmetic<std::decay_t<T>>::value
	&& !std::is_same<std::decay_t<T>, bool>::value >>
{
public:
	static std::string Cast(const T& value)
	{
		return std::to_string(value);
	}
};

//
// Declaration of the TypeToString class.
// It's a specialization for the bool type.
//

template<typename T>
class TypeToString<T, EnableIfSame_T<T, bool>>
{
public:
	static std::string Cast(const bool& value)
	{
		return value ? "true" : "false";
	}
};

//
// Declaration of the TypeToString class.
// It's a specialization for the pointer types (except const char*).
//

template<typename T>
class TypeToString<T, std::enable_if_t<std::is_pointer<std::decay_t<T>>::value
	&& !std::is_same<std::decay_t<T>, char*>::value
	&& !std::is_same<std::decay_t<T>, const char*>::value>>
{
public:
	static std::string Cast(const T& value)
	{
		if (value == nullptr)
		{
			return "null";
		}

		return std::string("(from pointer) ") + TypeToString<decltype(*value)>::Cast(*value);
	}
};

//
// Declaration of the TypeToString class.
// It's a specialization for the const char* type.
//

template<typename T>
class TypeToString<T, std::enable_if_t<std::is_same<std::decay_t<T>, char*>::value
	|| std::is_same<std::decay_t<T>, const char*>::value>>
{
public:
	static std::string Cast(const T& value)
	{
		if (value == nullptr)
		{
			return "null";
		}

		return std::string("(from pointer) ") + value;
	}
};

//
// Declaration of the TypeToString class.
// It's a specialization for the class types.
// 

template<typename T>
class TypeToString<T, std::enable_if_t<std::is_class<std::decay_t<T>>::value>>
{
public:
	static std::string Cast(const T& value)
	{
		std::string resStr;

		resStr.append("(").append(ClassToString<std::decay_t<T>>::GetClassName())
			.append(") : {").append(ClassToString<std::decay_t<T>>::GetValue(value)).append("}");

		return resStr;
	}
};

//
// Declaration of the ClassToString<std::string> class. 
//

template<>
class ClassToString<std::string>
{
public:
	static std::string GetClassName()
	{
		return "std::string";
	}

	static std::string GetValue(const std::string& value)
	{
		return value;
	}
};

template<typename T>
class ClassToString<std::vector<T>>
{
public:
	static std::string GetClassName()
	{
		return "std::vector<T>";
	}

	static std::string GetValue(const std::vector<T>& value)
	{
		std::string resStr;

		resStr.append("[");

		if (!value.empty())
		{
			resStr += TypeToString<T>::Cast(value[0]);

			size_t i = 1;
			for (; i < value.size() && i < Internal::MaxSequenceDisplayLength; ++i)
			{
				resStr.append(", ").append(TypeToString<T>::Cast(value[i]));
			}

			if (i != value.size())
			{
				resStr.append(", ...");
			}
		}
		
		resStr.append("]");

		return resStr;
	}
};

} // namespace StateInfo
