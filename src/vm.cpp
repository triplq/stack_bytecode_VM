#include "vm.hpp"

std::vector<int> Vm::get_stack() { return _stack; };

int Vm::run() {
	unsigned int pc = 0;

	while(pc != _code.size()) {
		switch(_code[pc]) {
		case PUSH:
			if (_stack.size() + 1 > kMaxStack) {
				throw VmError("stack overflow");
			}
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
			_stack.push_back(_stack[_stack.size() - 1] - _stack[_stack.size() - 2]);
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