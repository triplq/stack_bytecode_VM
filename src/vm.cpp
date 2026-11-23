#include "vm.hpp"

std::vector<int> Vm::get_stack() { return _stack; };

int Vm::run() {
	unsigned int pc = 0;

	while(pc != _code.size()) {
		switch(_code[pc]) {
		case PUSH:
			if (_stack.size() + 1 > kMaxStack) 
				throw VmError("stack overflow");
			_stack.push_back(_code[pc+1]);
			pc += 2;
			break;
		case ADD:
			if (_stack.size() < 2)
				throw VmError("stack underflow");
			_stack.push_back(_stack[_stack.size() - 1] + _stack[_stack.size() - 2]);
			pc++;
			break;
		case SUB:
			if (_stack.size() < 2)
				throw VmError("stack underflow");
			_stack.push_back(_stack[_stack.size() - 2] - _stack[_stack.size() - 1]);
			pc++;
			break;
		case MUL:
			if (_stack.size() < 2)
				throw VmError("stack underflow");
			_stack.push_back(_stack[_stack.size() - 1] * _stack[_stack.size() - 2]);
			pc++;
			break;
		case DIV:
			if (_stack.size() < 2)
				throw VmError("stack underflow");
			if (_stack[_stack.size() - 1] == 0)
				throw VmError("division by zero");
			_stack.push_back(_stack[_stack.size() - 2] / _stack[_stack.size() - 1]);
			pc++;
			break;
		case DUP:
			if (_stack.size() + 1 > kMaxStack)
				throw VmError("stack overflow");
			if (_stack.size() == 0)
				throw VmError("stack underflow");
			_stack.push_back(_stack.back());
			pc++;
			break;
		case SWAP:
			if (_stack.size() < 2)
				throw VmError("stack underflow");
			std::swap(_stack[_stack.size() - 1], _stack[_stack.size() - 2]);
			pc++;
			break;
		case POP:
			if (_stack.size() < 1)
				throw VmError("stack underflow");
			_stack.pop_back();
			pc++;
			break;
		case HALT:
			return _stack.back();
		default:
			throw VmError("not an action");
		}
	}

	return 0;
};