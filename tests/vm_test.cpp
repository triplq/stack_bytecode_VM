#include <exception>
#include <gtest/gtest.h>
#include <stdexcept>
#include "vm.hpp"

int exec(std::vector<int> code) {
	Vm virtmachine(std::move(code));
	return virtmachine.run();
}

TEST(StartText, MachineOutput) {
	EXPECT_EQ(exec({Vm::PUSH, 5, Vm::PUSH, 7, Vm::ADD, Vm::HALT}), 12);
}

TEST(StartText, FailureOutput) {
	try {
		exec({Vm::PUSH, 5, Vm::ADD, Vm::HALT});
		FAIL() << "Expected std::runtime_error";
	} catch(const std::runtime_error& err) {
		EXPECT_STREQ(err.what(), "stack underflow");
	} catch(...) {
		FAIL() << "other type of error";
	}
}