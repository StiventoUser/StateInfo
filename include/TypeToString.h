#pragma once

namespace StateInfo
{

//! An alias of type with removed reference, const, volatile.
template<typename T>
using RemoveRefCv_T = std::remove_cv_t<std::remove_reference_t<T>>;

//! An alias of type that exists if 2 types are same (with ignored qualifiers)
template<typename T, typename ExpectedT>
using EnableIfSame_T = std::enable_if_t<std::is_same<RemoveRefCv_T<T>, RemoveRefCv_T<ExpectedT>>::value>;

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
class TypeToString<T, std::enable_if_t<std::is_arithmetic<RemoveRefCv_T<T>>::value &&
	!std::is_same<RemoveRefCv_T<T>, bool>::value >>
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
class TypeToString<T, std::enable_if_t<std::is_pointer<RemoveRefCv_T<T>>::value
	&& !std::is_same<RemoveRefCv_T<T>, const char*>::value>>
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
class TypeToString<T, std::enable_if_t<std::is_same<RemoveRefCv_T<T>, const char*>::value>>
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
class TypeToString<T, std::enable_if_t<std::is_class<RemoveRefCv_T<T>>::value>>
{
public:
	static std::string Cast(const T& value)
	{
		std::string resStr;

		resStr.append("(").append(ClassToString<RemoveRefCv_T<T>>::GetClassName())
			.append(") : {").append(ClassToString<RemoveRefCv_T<T>>::GetValue(value)).append("}");

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

} // namespace StateInfo
