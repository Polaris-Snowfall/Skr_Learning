import lit
import lit.llvm

lit.llvm.initialize(lit_config, config)

from lit.llvm import llvm_config


config.name = "LocalOpts"
config.test_format = lit.formats.ShTest()
config.test_source_root = "/mnt/test"
config.test_exec_root = "/mnt/test"
config.suffixes = [".c", ".ll"]

config.substitutions.append((r"%dylibdir", "/mnt/lib"))

config.llvm_config_bindir = "/usr/lib/llvm-16/bin"
llvm_config.add_tool_substitutions(["opt", "FileCheck"], config.llvm_config_bindir)
