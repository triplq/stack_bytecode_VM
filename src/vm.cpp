#include "vm.hpp"

std::vector<int> Vm::get_stack() { return _stack; };

int Vm::run() {
	unsigned int pc = 0;
	unsigned int steps = 0;

	auto push = [&](int a){
		if (_stack.size() + 1 > kMaxStack) 
			throw VmError("stack overflow");
		_stack.push_back(a);
	};

	auto pop = [&]() -> int{
		if (_stack.size() < 1)
			throw VmError("stack underflow");
		int res = _stack.back();
		_stack.pop_back();
		return res;
	};

	if (_code.size() == 1) 
		throw VmError("code underflow");

	while(pc != _code.size()) {
		if (steps >= _maxSteps)
			throw VmError("steps overflow");
		switch(_code[pc]) {
		case PUSH: {
			push(_code[pc+1]);
			pc += 2;
			break;
		}
		case ADD: {
			int a = pop(), b = pop();
			push(b + a);
			pc++;
			break;
		}
		case SUB: {
			int a = pop(), b = pop();
			push(b - a);
			pc++;
			break;
		}
		case MUL: {
			int a = pop(), b = pop();
			push(b * a);
			pc++;
			break;
		}
		case DIV: {
			int a = pop(), b = pop();
			if (a == 0)
				throw VmError("division by zero");
			push(b / a);
			pc++;
			break;
		}
		case DUP: {
			if (_stack.size() == 0)
				throw VmError("stack underflow");
			push(_stack.back());
			pc++;
			break;
		}
		case SWAP: {
			if (_stack.size() < 2)
				throw VmError("stack underflow");
			std::swap(_stack[_stack.size() - 1], _stack[_stack.size() - 2]);
			pc++;
			break;
		}
		case POP: {
			pop();
			pc++;
			break;
		}
		case LOAD: {
			if (_local.size() == 0)
				throw VmError("local underflow");
			push(_local.back());
			_local.pop_back();
			pc++;
			break;
		}
	 	case STORE: {
			if (_local.size() + 1 > kMaxLocal)
				throw VmError("local overflow");
			_local.push_back(_code[pc + 1]);
			pc += 2;
			break;
	 	}
		case JMP: {
			if (_code[pc + 1] > _code.size() - 1)
				throw VmError("wrong address to jmp");
			pc = _code[pc + 1];
			break;
		}
		case JZ: {
			int a = pop();
			if (a == 0) {
				if (_code[pc + 1] > _code.size() - 1)
					throw VmError("wrong address to jmp");
				pc = _code[pc + 1];
			} else {
				pc += 2;
			}
			break;
		}
		case HALT: {
			return _stack.back();
		}
		default: {
			throw VmError("not an action");
		}
		}
		steps++;
	}

	return 0;
};