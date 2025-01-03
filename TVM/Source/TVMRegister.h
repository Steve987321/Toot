#pragma once

namespace TVM
{
	enum RegisterType
	{
		REGISTER, // for args
		FUNCTION, // for parser
		STRING,
		FLOAT,
		INT,
	};

	union RegisterValue
	{
		int32_t num;
		float flt;
		const char* str;
	};

	// Register value info 
	struct Register
	{
		RegisterValue value;
		RegisterType type;
	};
}
