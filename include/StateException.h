#pragma once

#include <boost/preprocessor/seq/for_each.hpp>

#include "TypeToString.h"

//#define STATEEXCEPT_NODEBUG

#ifndef STATEEXCEPT_NODEBUG

//! Makes new StateException exception, fills with data and prepares it before throw.
#define STATEEXCEPT_NEW(message, ...) StateInfo::StateException(message, __FILE__, __LINE__)\
	STATEEXCEPT_USE_IMPL(__VA_ARGS__).LockState()

//! Makes new StateException exception based on other exception, fills with data and prepares it before throw.
#define STATEEXCEPT_WRAP(message, e, ...) StateInfo::StateException(message, e, __FILE__, __LINE__)\
	STATEEXCEPT_USE_IMPL(__VA_ARGS__).LockState()

//! Makes new StateException exception using STATEEXCEPT_WRAP and throws it.
#define STATEEXCEPT_WRAP_AND_RETHROW(message, e, ...) throw STATEEXCEPT_WRAP(message, e, __VA_ARGS__)

//! Catches passed type, creates StateException and throws it.
#define STATEEXCEPT_CATCH_AND_RETHROW(catchedType, message, ...) catch (const catchedType& e) {\
	throw STATEEXCEPT_WRAP(message, e, __VA_ARGS__); }

//! Catches passed type, creates StateException and throws it.
#define STATEEXCEPT_CATCH_AND_RETHROW_DEBUG(catchedType, message, ...) catch (const catchedType& e) {\
	throw STATEEXCEPT_WRAP(message, e, __VA_ARGS__); }

//! Formats all passed class variables.
#define STATEEXCEPT_FORMATCLASS(obj, ...) std::string() BOOST_PP_SEQ_FOR_EACH(STATEEXCEPT_FORMATVAR, obj, __VA_ARGS__)

//
// Internal macroses.
//

//! Passes variables sequence to boost.
#define STATEEXCEPT_USE_IMPL(...) BOOST_PP_SEQ_FOR_EACH(STATEEXCEPT_USE_IMPL_APPEND, _, __VA_ARGS__)

//! Passes variable that is got from boost for each macro to implementation macro.
#define STATEEXCEPT_USE_IMPL_APPEND(r, data, var) STATEEXCEPT_USE_IMPL_APPEND_IMPL(r, data, var)
//! Adds passed variable to StateException exception.
#define STATEEXCEPT_USE_IMPL_APPEND_IMPL(r, data, var) .AddVariable(#var, var)

//! Passes arguments from boost for each to STATEEXCEPT_FORMATVAR_IMPL.
#define STATEEXCEPT_FORMATVAR(r, obj, var) STATEEXCEPT_FORMATVAR_IMPL(r, obj, var)
//! Appends variable name and value to exception info.
#define STATEEXCEPT_FORMATVAR_IMPL(r, obj, var) .append(#var).append(" = ").append(TypeToString<decltype(obj.var)>::Cast(obj.var)).append(";")

#else // STATEEXCEPT_NODEBUG defined

//! With STATEEXCEPT_NODEBUG StateException is changed to std::runtime_error.
#define STATEEXCEPT_NEW(message, ...) std::runtime_error(message)
//! With STATEEXCEPT_NODEBUG StateException is changed to std::runtime_error.
#define STATEEXCEPT_WRAP(message, e, ...) std::runtime_error(message)

//! With STATEEXCEPT_NODEBUG rethrows original exception.
#define STATEEXCEPT_WRAP_AND_RETHROW(message, e, ...) throw

//! With STATEEXCEPT_NODEBUG catches passed type and throws an STATEEXCEPT_NEW exception.
#define STATEEXCEPT_CATCH_AND_RETHROW(catchedType, message, ...) catch (const catchedType& e) {\
	throw STATEEXCEPT_NEW(message, __VA_ARGS__); }

//! With STATEEXCEPT_NODEBUG the catch block never affects a program.
#define STATEEXCEPT_CATCH_AND_RETHROW_DEBUG(catchedType, message, ...)\
	catch (const StateInfo::Internal::NotExistedException&) {}

//! With STATEEXCEPT_NODEBUG does nothing.
#define STATEEXCEPT_FORMATCLASS(obj, ...) std::string()

#endif

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

	varsValues_.append("\n\t").append(varName).append(" = ").append(TypeToString<T>::Cast(varValue));

	return *this;
}

namespace Internal
{

//
// Declaration of the NotExistedException class.
// Never use it in your code.
//

class NotExistedException : std::exception
{
};

} // namespace StateInfo

} // namespace StateInfo
