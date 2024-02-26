	.text
	.file	"TestCase1.ll"
	.globl	foo                             # -- Begin function foo
	.p2align	4, 0x90
	.type	foo,@function
foo:                                    # @foo
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$7056, %eax                     # imm = 0x1B90
	xorl	%edx, %edx
	idivl	%edi
	imull	$54, %eax, %ecx
	movslq	%ecx, %r10
	imulq	$1272582903, %r10, %r11         # imm = 0x4BDA12F7
	movq	%r11, %rcx
	shrq	$63, %rcx
	sarq	$36, %r11
	addl	%ecx, %r11d
	subq	$8, %rsp
	.cfi_adjust_cfa_offset 8
	movl	$.L.str, %edi
	movl	$21, %esi
	movl	$336, %edx                      # imm = 0x150
	movl	$7056, %ecx                     # imm = 0x1B90
	movl	%eax, %r8d
	movl	$705, %r9d                      # imm = 0x2C1
	xorl	%eax, %eax
	pushq	$54
	.cfi_adjust_cfa_offset 8
	pushq	$1
	.cfi_adjust_cfa_offset 8
	pushq	%r11
	.cfi_adjust_cfa_offset 8
	pushq	$5
	.cfi_adjust_cfa_offset 8
	pushq	%r10
	.cfi_adjust_cfa_offset 8
	callq	printf@PLT
	addq	$48, %rsp
	.cfi_adjust_cfa_offset -48
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	foo, .Lfunc_end0-foo
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n"
	.size	.L.str, 31

	.section	".note.GNU-stack","",@progbits
