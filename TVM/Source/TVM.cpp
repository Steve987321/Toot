#include "TVM.h"

#include <cassert>
#include <format>
#include <string>
#include <iostream>

void VM::Init()
{
	// #todo #include brrr
	IO::RegisterToVM(*this);

	rel_reg_stack.push(0);
}

void VM::Run()
{
	while (instruction_pointer < instructions.size())
	{
		Instruction& i = instructions[instruction_pointer];

		switch (i.op)
		{
		case OP_LABEL:
			// add to functions or if just label add to somethign eles
			assert(i.args.size() == 1);
			OpLabel(i.args[0]);
			break;
		case OP_MOVE:
			assert(i.args.size() == 2);
			OpMove(i.args[0], i.args[1]);
			break;
		case OP_ADD:
			assert(i.args.size() == 3);
			OpAdd(i.args[0], i.args[1], i.args[2]);
			break;
		case OP_SUBTRACT:
			assert(i.args.size() == 3);
			OpSubtract(i.args[0], i.args[1], i.args[2]);
			break;
		case OP_MULTIPLY:
			assert(i.args.size() == 3);
			OpMultiply(i.args[0], i.args[1], i.args[2]);
			break;
		case OP_DIVIDE:
			assert(i.args.size() == 3);
			OpDivide(i.args[0], i.args[1], i.args[2]);
			break;
		//case OP_NEGATE:
		//	assert(i.args.size() == 2);
		//	OpNegate(i.args[0], i.args[1]);
		//	break;
		case OP_NOT:
			break;
		case OP_CALL:
			OpCall(i.args);
			break;
		case OP_JUMP:
			assert(i.args.size() == 1);
			OpJump(i.args[0]);
			break;
		case OP_JUMPIFNOTZERO:
			break;
		case OP_JUMPIFZERO:
			break;
		case OP_RETURN:
			break;
		default: 
			break;
		}

		instruction_pointer++;
	}
}

void VM::OpMove(const VMRegister& dst, const VMRegister& src)
{
	assert(dst.type == REGISTER);
	VMRegister& dst_reg = registers[dst.value.num + relative_register_index];
	dst_reg = src;
}

void VM::OpAdd(const VMRegister& dst, const VMRegister& a, const VMRegister& b)
{
	VMRegister* l = nullptr;
	VMRegister* r = nullptr;
	if (a.type == REGISTER)
		l = &registers[a.value.num + relative_register_index];
	if (b.type == REGISTER)
		r = &registers[b.value.num + relative_register_index];

	assert(l && r);

	VMRegister* dst_reg = &registers[dst.value.num + relative_register_index];
	//*dst_reg = *l;

	switch (l->type)
	{
	case STRING:
		break;
	case FLOAT:
		dst_reg->type = FLOAT;
		dst_reg->value.flt = l->value.num + r->value.flt;
		break;
	case INT:
		dst_reg->type = INT;
		dst_reg->value.num = l->value.num + r->value.num;
		break;
	default:
		break;
	}
}

void VM::OpSubtract(const VMRegister& dst, const VMRegister& a, const VMRegister& b)
{
	VMRegister* l = nullptr;
	VMRegister* r = nullptr;
	if (a.type == REGISTER)
		l = &registers[a.value.num + relative_register_index];
	if (b.type == REGISTER)
		r = &registers[b.value.num + relative_register_index];

	assert(l && r);

	VMRegister* dst_reg = &registers[dst.value.num + relative_register_index];
	//*dst_reg = *l;

	switch (l->type)
	{
	case STRING:
		break;
	case FLOAT:
		dst_reg->type = FLOAT;
		dst_reg->value.flt = l->value.flt - r->value.flt;
		break;
	case INT:
		dst_reg->type = INT;
		dst_reg->value.num = l->value.num - r->value.num;
		break;
	default:
		break;
	}
}

void VM::OpMultiply(const VMRegister& dst, const VMRegister& a, const VMRegister& b)
{
	VMRegister* l = nullptr;
	VMRegister* r = nullptr;
	if (a.type == REGISTER)
		l = &registers[a.value.num + relative_register_index];
	if (b.type == REGISTER)
		r = &registers[b.value.num + relative_register_index];

	assert(l && r);

	VMRegister* dst_reg = &registers[dst.value.num + relative_register_index];
	//*dst_reg = *l;

	switch (l->type)
	{
	case FLOAT:
		dst_reg->type = FLOAT;
		dst_reg->value.flt = l->value.flt * r->value.flt;
		break;
	case INT:
		dst_reg->type = INT;
		dst_reg->value.num = l->value.num * r->value.num;
		break;
	default:
		break;
	}
}

void VM::OpDivide(const VMRegister& dst, const VMRegister& a, const VMRegister& b)
{
	VMRegister* l = nullptr;
	VMRegister* r = nullptr;
	if (a.type == REGISTER)
		l = &registers[a.value.num + relative_register_index];
	if (b.type == REGISTER)
		r = &registers[b.value.num + relative_register_index];

	assert(l && r);

	VMRegister* dst_reg = &registers[dst.value.num + relative_register_index];
	*dst_reg = *l;

	switch (l->type)
	{
	case FLOAT:
		dst_reg->type = FLOAT;
		dst_reg->value.flt = l->value.flt / r->value.flt;
		break;
	case INT:
		dst_reg->type = INT;
		dst_reg->value.num = l->value.num / r->value.num;
		break;
	default:
		break;
	}
}

void VM::OpLabel(const VMRegister& a)
{
	labels[a.value.str] = instruction_pointer;
}

void VM::OpCall(const std::vector<VMRegister>& args)
{
	BeginRelativeRegIndex(instruction_pointer);

	auto function_it = functions.find(args[0].value.str);
	if (function_it != functions.end())
		function_it->second.func(*this, args);
	else
	{
		auto labels_it = labels.find(args[0].value.str);
		if (labels_it != labels.end())
		{
			// go to label 
			instruction_pointer = labels_it->second;
		}
		else
		{
			throw std::runtime_error("no function like that");
		}
	}

	EndRelativeRegIndex();
}

VMRegister& VM::GetReg(uint64_t index)
{
	assert(index + relative_register_index < registers.size() && "iNCcrease Vm Registers size");

	return registers[index + relative_register_index];
}

//Register& VM::GetValueAtReg()
//{
//
//}

void VM::OpJump(const VMRegister& a)
{
	instruction_pointer = a.value.num;
}

void VM::BeginRelativeRegIndex(uint64_t rel)
{
	relative_register_index += rel_reg_stack.top();
	rel_reg_stack.push(relative_register_index);
}

void VM::EndRelativeRegIndex()
{
	// #TODO: uwnind registers from previous call

	relative_register_index -= rel_reg_stack.top();
	rel_reg_stack.pop();
}

