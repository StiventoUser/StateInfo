#pragma once

#include "TypeToString.h"

#define STATEEXCEPT_NEW(message) StateInfo::StateException(message, __FILE__, __LINE__)
#define STATEEXCEPT_WRAP(message, e) StateInfo::StateException(message, e, __FILE__, __LINE__)
#define STATEEXCEPT_USE(var) AddVariable(#var, var)
#define STATEEXCEPT_FORMATVAR(var) append(#var).append(" = ").append(TypeToString<decltype(var)>::Cast(var))

namespace StateInfo
{

//
// Declaration of the StateException class.
//

class StateException : public std::exception
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	StateException(const char* message, const char* file, const int line);
	//! Constructor.
	StateException(const char* message, const std::exception& e, const char* file, const int line);

	//
	// Public interface.
	//
public:
	//! Adds a variable value as exception additional parameter.
	template<typename T>
	StateException& AddVariable(const char* varName, const T& varValue);
	//! Locks state of the exception and prepares the message.
	StateException& LockState();

	//! Returns a full message with debug info.
	virtual const char* what() const override;
	//! Returns a full message with debug info.
	const std::string& GetFullMessage() const;
	//! Returns the last message without debug info.
	const std::string& GetLastMessage() const;

	//
	// Private date members.
	//
private:
	//! Exception thrown at file.
	const char* file_;
	//! Exception thrown at line.
	const int line_;
	//! Exception message.
	std::string message_;
	//! Nested exception message.
	std::string nestedExceptionMessage_;
	//! Variables value info.
	std::string varsValues_;
	//! Full exception message.
	std::string fullMessage_;
};

//
// Definitions of the StateException class.
//

template<typename T>
StateException& StateException::AddVariable(const char* varName, const T& varValue)
{
	if (!varsValues_.empty())
	{
		varsValues_.append(", ");
	}

	varsValues_.append(varName).append(" = ").append(TypeToString<T>::Cast(varValue));

	return *this;
}

} // namespace StateInfo
