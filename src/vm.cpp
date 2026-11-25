#include "vm.hpp"
#include <boost/safe_numerics/safe_integer.hpp>
#include <exception>

using namespace boost::safe_numerics;

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

	auto get_next = [&](const std::vector<int>& vec, int i) -> int{
		if (vec.size() == i + 1)
			throw VmError("heap buffer overflow");
		return vec[i + 1];
	};

	while(pc != _code.size()) {
		if (steps >= _maxSteps)
			throw VmError("steps overflow");
		switch(_code[pc]) {
		case PUSH: {
			push(get_next(_code, pc));
			pc += 2;
			break;
		}
		case ADD: {
			int a = pop(), b = pop();
			try {
				push(safe<int>(b) + safe<int>(a));
			} catch(const std::exception& e) {
				throw VmError("int overflow");
			}
			pc++;
			break;
		}
		case SUB: {
			int a = pop(), b = pop();
			try {
				push(safe<int>(b) - safe<int>(a));
			} catch(const std::exception& e) {
				throw VmError("int overflow");
			}
			pc++;
			break;
		}
		case MUL: {
			int a = pop(), b = pop();
			try {
				push(safe<int>(b) * safe<int>(a));
			} catch(const std::exception& e) {
				throw VmError("int overflow");
			}
			pc++;
			break;
		}
		case DIV: {
			int a = pop(), b = pop();
			if (a == 0)
				throw VmError("division by zero");
			try {
				push(safe<int>(b) / safe<int>(a));
			} catch(const std::exception& e) {
				throw VmError("int overflow");
			}
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
			_local.push_back(get_next(_code, pc));
			pc += 2;
			break;
	 	}
		case JMP: {
			if (get_next(_code, pc) > _code.size() - 1 || get_next(_code, pc) < 0)
				throw VmError("wrong address to jmp");
			pc = get_next(_code, pc);
			break;
		}
		case JZ: {
			int a = pop();
			if (a == 0) {
				if (get_next(_code, pc) > _code.size() - 1 || get_next(_code, pc) < 0) 
					throw VmError("wrong address to jmp");
				pc = get_next(_code, pc);
			} else {
				pc += 2;
			}
			break;
		}
		case HALT: {
			if (_stack.size() == 0)
				throw VmError("stack is empty");
			return _stack.back();
		}
		default: {
			throw VmError("not an action");
		}
		}
		steps++;
	}

	throw VmError("programm end without HALT");
};