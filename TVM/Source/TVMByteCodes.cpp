#include "TVMByteCodes.h"

namespace TVM
{

	void Add()
	{
		
	}

	void InitBytecodes()
	{
		bytecodes[0] = {OP_CODE::OP_ADD, Add};
	}

}
