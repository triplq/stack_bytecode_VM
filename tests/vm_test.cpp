#include <gtest/gtest.h>
#include "vm.hpp"

int exec(std::vector<int> code) {
	Vm virtmachine(std::move(code));
	return virtmachine.run();
}

TEST(StartText, MachineOutput) {
	EXPECT_EQ(exec({Vm::PUSH, 5, Vm::PUSH, 7, Vm::ADD, Vm::HALT}), 12);
}