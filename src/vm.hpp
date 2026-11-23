#pragma once

#include <stdexcept>
#include <vector>

class Vm {
private:
	std::vector<int> _code;
	std::vector<int> _local;
	std::vector<int> _stack;
	const unsigned int _maxSteps;

public:
	static constexpr unsigned int kMaxStack = 256;
	static constexpr unsigned int kMaxLocal = 8;

	enum actions {PUSH = 1, ADD, SUB, HALT};

	class VmError : public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	explicit Vm(std::vector<int> code, unsigned int MaxSteps = 1'000'000) : _code(std::move(code)), _maxSteps(MaxSteps) {};

	int run();
	std::vector<int> get_stack();
};