#include <exception>
#include <string>
#include <vector>
#include <type_traits>

#include "StateException.h"

namespace StateInfo
{

StateException::StateException(const char* message, const char* file, const int line)
	: message_(message)
	, file_(file)
	, line_(line)
{

}

StateException::StateException(const char* message, const std::exception& e,
	const char* file, const int line)
	: message_(message)
	, nestedExceptionMessage_((e.what() == nullptr) ? "" : e.what())
	, file_(file)
	, line_(line)
{

}

StateException& StateException::LockState()
{
	fullMessage_.append("\"").append(message_).append("\"")
		.append(" at ").append(file_).append(":").append(std::to_string(line_));

	if (!varsValues_.empty())
	{
		fullMessage_.append(" with variables: [ ")
			.append(varsValues_)
			.append(" ]");
	}
	if (!nestedExceptionMessage_.empty())
	{
		fullMessage_.append(" which is caused by:\n")
			.append(nestedExceptionMessage_);
	}

	return *this;
}

const char* StateException::what() const
{
	return fullMessage_.c_str();
}

const std::string& StateException::GetFullMessage() const
{
	return fullMessage_;
}

const std::string& StateException::GetLastMessage() const
{
	return message_;
}

} // namespace StateInfo
