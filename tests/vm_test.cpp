#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <gmock/gmock.h>
#include "vm.hpp"

int exec(std::vector<int> code, int steps = 1'000'000) {
	Vm virtmachine(std::move(code), steps);
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

TEST(StartText, Div) {
	EXPECT_THROW(exec({Vm::PUSH, 6, Vm::PUSH, 0, Vm::DIV, Vm::HALT}), std::runtime_error);
}

TEST(StartText, check) {
	EXPECT_THROW(exec({Vm::PUSH}), std::runtime_error);
}

TEST(StartText, DupSwapPop) {
	EXPECT_THROW(exec({Vm::DUP}), std::runtime_error);
	EXPECT_THROW(exec({Vm::SWAP}), std::runtime_error);
	EXPECT_THROW(exec({Vm::POP}), std::runtime_error);
}

TEST(StartText, LoadStore) {
	EXPECT_THROW(exec({Vm::STORE}), std::runtime_error);
	EXPECT_THROW(exec({Vm::LOAD}), std::runtime_error);
	EXPECT_THAT([&](){ exec({Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5, Vm::STORE, 5}); }, testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("local overflow")));
	EXPECT_EQ(exec({Vm::STORE, 5, Vm::LOAD, Vm::STORE, 1, Vm::LOAD, Vm::ADD, Vm::HALT}), 6);
}

TEST(StartText, JmpJz) {
	EXPECT_THROW(exec({Vm::JMP, 5}), std::runtime_error);
	EXPECT_THAT([&](){ exec({Vm::PUSH, 0, Vm::JZ, 5}); }, testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("wrong address to jmp")));
	EXPECT_THAT([&](){ exec({Vm::JMP}); }, testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("code underflow")));
	EXPECT_THAT([&](){ exec({Vm::JMP, 0}); }, testing::ThrowsMessage<std::runtime_error>(testing::HasSubstr("steps overflow")));
}