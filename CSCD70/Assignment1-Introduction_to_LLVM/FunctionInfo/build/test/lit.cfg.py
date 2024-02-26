# *lit* is the LLVM integrated tester. Please visit
#
#     https://llvm.org/docs/CommandGuide/lit.html
#
# for its documentation.
import lit
import lit.llvm

lit.llvm.initialize(lit_config, config)

from lit.llvm import llvm_config


config.name = "FunctionInfo"
# The ShTest files contain some number of shell-like command pipelines, along
# with assertions about what should be in the output.
config.test_format = lit.formats.ShTest()
config.test_source_root = "/mnt/FunctionInfo/test"
config.test_exec_root = "/mnt/FunctionInfo/build/test"
config.suffixes = [".c", ".ll"]

config.substitutions.append((r"%dylibdir", "/mnt/FunctionInfo/build/lib"))

# Extra config attributes for the tool substitution.
config.llvm_config_bindir = "/usr/lib/llvm-16/bin"
llvm_config.add_tool_substitutions(
    ["clang", "opt", "FileCheck"], config.llvm_config_bindir
)
