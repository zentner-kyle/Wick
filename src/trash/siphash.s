	.file	"siphash.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"OK"
.LC1:
	.string	"FAIL"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB18:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	xorl	%eax, %eax
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movl	$vectors.1794, %r12d
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$152, %rsp
	.cfi_def_cfa_offset 208
	leaq	48(%rsp), %r15
	leaq	80(%rsp), %r14
	leaq	144(%rsp), %r8
	leaq	16(%rsp), %r13
	leaq	56(%rsp), %rbp
	movq	%r8, 8(%rsp)
	movq	%r14, %r10
	movq	%r15, %r11
	.p2align 4,,10
	.p2align 3
.L18:
	movl	%r10d, %edx
	movq	%r13, %rdi
	movl	$7, %ecx
	subl	%r14d, %edx
	movabsq	$7746018054687388513, %rsi
	leaq	16(%rsp), %r13
	rep stosq
	movb	%dl, (%r10)
	movabsq	$8893317812261383291, %rdi
	movabsq	$8388350548712186997, %rdx
	movabsq	$7737605742629119589, %rcx
	movq	%rdi, 40(%rsp)
	movq	%rdx, 16(%rsp)
	movq	%rcx, 24(%rsp)
	movq	%rsi, 32(%rsp)
	movq	%r11, %rbx
	movq	%r11, 56(%rsp)
	movq	%r14, %rdi
	.p2align 4,,10
	.p2align 3
.L23:
	cmpq	%rdi, %r10
	jbe	.L3
.L27:
	cmpq	%rbp, %rbx
	jae	.L5
	movzbl	(%rdi), %edx
	addq	$1, %rdi
	movb	%dl, (%rbx)
	addq	$1, %rbx
	cmpq	%rdi, %r10
	movq	%rbx, 56(%rsp)
	ja	.L27
.L3:
	cmpq	%rbp, %rbx
	jb	.L28
.L5:
	movzbl	49(%rsp), %edx
	movzbl	50(%rsp), %ecx
	movq	%r11, %rbx
	movq	%r11, 56(%rsp)
	salq	$16, %rcx
	salq	$8, %rdx
	orq	%rcx, %rdx
	movzbl	48(%rsp), %ecx
	orq	%rcx, %rdx
	movzbl	51(%rsp), %ecx
	salq	$24, %rcx
	orq	%rcx, %rdx
	movzbl	52(%rsp), %ecx
	salq	$32, %rcx
	orq	%rcx, %rdx
	movzbl	53(%rsp), %ecx
	salq	$40, %rcx
	orq	%rcx, %rdx
	movzbl	54(%rsp), %ecx
	salq	$48, %rcx
	orq	%rcx, %rdx
	movzbl	55(%rsp), %ecx
	salq	$56, %rcx
	orq	%rcx, %rdx
	movq	24(%rsp), %rcx
	movq	%rdx, %rsi
	xorq	40(%rsp), %rsi
	movq	%rcx, %r8
	addq	16(%rsp), %r8
	rorq	$51, %rcx
	movq	%rsi, %r9
	addq	32(%rsp), %r9
	rorq	$48, %rsi
	xorq	%r8, %rcx
	rorq	$32, %r8
	xorq	%r9, %rsi
	addq	%rcx, %r9
	rorq	$47, %rcx
	addq	%rsi, %r8
	xorq	%r9, %rcx
	rorq	$43, %rsi
	xorq	%r8, %rsi
	rorq	$32, %r9
	addq	%rcx, %r8
	rorq	$51, %rcx
	addq	%rsi, %r9
	xorq	%r8, %rcx
	rorq	$48, %rsi
	rorq	$32, %r8
	xorq	%r9, %rsi
	addq	%rcx, %r9
	rorq	$47, %rcx
	addq	%rsi, %r8
	rorq	$43, %rsi
	xorq	%r9, %rcx
	xorq	%r8, %rsi
	xorq	%r8, %rdx
	movq	64(%rsp), %r8
	rorq	$32, %r9
	movq	%rsi, 40(%rsp)
	movq	%rcx, 24(%rsp)
	movq	%r9, 32(%rsp)
	movq	%rdx, 16(%rsp)
	addq	$8, %r8
	cmpq	%rdi, %r10
	movq	%r8, 64(%rsp)
	ja	.L23
.L6:
	subl	%r15d, %ebx
	leaq	(%r8,%rbx), %rdi
	salq	$56, %rdi
	cmpb	$7, %bl
	ja	.L8
	movzbl	%bl, %ebx
	jmp	*.L16(,%rbx,8)
	.section	.rodata
	.align 8
	.align 4
.L16:
	.quad	.L8
	.quad	.L9
	.quad	.L10
	.quad	.L11
	.quad	.L12
	.quad	.L13
	.quad	.L14
	.quad	.L15
	.section	.text.startup
.L15:
	movzbl	54(%rsp), %r8d
	salq	$48, %r8
	orq	%r8, %rdi
.L14:
	movzbl	53(%rsp), %r8d
	salq	$40, %r8
	orq	%r8, %rdi
.L13:
	movzbl	52(%rsp), %r8d
	salq	$32, %r8
	orq	%r8, %rdi
.L12:
	movzbl	51(%rsp), %r8d
	salq	$24, %r8
	orq	%r8, %rdi
.L11:
	movzbl	50(%rsp), %r8d
	salq	$16, %r8
	orq	%r8, %rdi
.L10:
	movzbl	49(%rsp), %r8d
	salq	$8, %r8
	orq	%r8, %rdi
.L9:
	movzbl	48(%rsp), %r8d
	orq	%r8, %rdi
.L8:
	xorq	%rdi, %rsi
	addq	%rcx, %rdx
	rorq	$51, %rcx
	addq	%rsi, %r9
	rorq	$48, %rsi
	xorq	%rdx, %rcx
	xorq	%r9, %rsi
	rorq	$32, %rdx
	addq	%rcx, %r9
	addq	%rsi, %rdx
	rorq	$47, %rcx
	rorq	$43, %rsi
	xorq	%rdx, %rsi
	xorq	%r9, %rcx
	rorq	$32, %r9
	leaq	(%rsi,%r9), %r8
	addq	%rcx, %rdx
	rorq	$48, %rsi
	rorq	$51, %rcx
	xorq	%rdx, %rcx
	xorq	%r8, %rsi
	rorq	$32, %rdx
	leaq	(%rsi,%rdx), %r9
	leaq	(%r8,%rcx), %rdx
	rorq	$47, %rcx
	rorq	$43, %rsi
	movzbl	2(%r12), %r8d
	xorq	%rdx, %rcx
	xorq	%r9, %rdi
	rorq	$32, %rdx
	xorq	%r9, %rsi
	addq	%rcx, %rdi
	xorb	$-1, %dl
	rorq	$51, %rcx
	addq	%rsi, %rdx
	rorq	$48, %rsi
	xorq	%rdi, %rcx
	xorq	%rdx, %rsi
	rorq	$32, %rdi
	leaq	(%rcx,%rdx), %r9
	addq	%rsi, %rdi
	rorq	$47, %rcx
	rorq	$43, %rsi
	movzbl	1(%r12), %edx
	xorq	%rdi, %rsi
	xorq	%r9, %rcx
	rorq	$32, %r9
	addq	%rcx, %rdi
	addq	%rsi, %r9
	rorq	$51, %rcx
	rorq	$48, %rsi
	xorq	%rdi, %rcx
	rorq	$32, %rdi
	xorq	%r9, %rsi
	addq	%rcx, %r9
	rorq	$47, %rcx
	addq	%rsi, %rdi
	rorq	$43, %rsi
	xorq	%r9, %rcx
	xorq	%rdi, %rsi
	rorq	$32, %r9
	addq	%rcx, %rdi
	addq	%rsi, %r9
	rorq	$51, %rcx
	rorq	$48, %rsi
	xorq	%r9, %rsi
	xorq	%rdi, %rcx
	rorq	$32, %rdi
	addq	%rsi, %rdi
	rorq	$43, %rsi
	addq	%rcx, %r9
	xorq	%rdi, %rsi
	rorq	$47, %rcx
	salq	$16, %r8
	salq	$8, %rdx
	xorq	%r9, %rcx
	rorq	$32, %r9
	orq	%r8, %rdx
	movzbl	(%r12), %r8d
	addq	%rcx, %rdi
	addq	%rsi, %r9
	rorq	$51, %rcx
	rorq	$48, %rsi
	xorq	%rdi, %rcx
	xorq	%r9, %rsi
	rorq	$32, %rdi
	addq	%rsi, %rdi
	addq	%rcx, %r9
	rorq	$43, %rsi
	orq	%r8, %rdx
	movzbl	3(%r12), %r8d
	rorq	$47, %rcx
	xorq	%r9, %rcx
	xorq	%rdi, %rsi
	rorq	$32, %r9
	movq	%rcx, 24(%rsp)
	xorq	%rsi, %rcx
	movq	%rdi, 16(%rsp)
	xorq	%rdi, %rcx
	movq	%rsi, 40(%rsp)
	movq	%r9, 32(%rsp)
	salq	$24, %r8
	orq	%r8, %rdx
	movzbl	4(%r12), %r8d
	salq	$32, %r8
	orq	%r8, %rdx
	movzbl	5(%r12), %r8d
	salq	$40, %r8
	orq	%r8, %rdx
	movzbl	6(%r12), %r8d
	salq	$48, %r8
	orq	%r8, %rdx
	movzbl	7(%r12), %r8d
	salq	$56, %r8
	orq	%r8, %rdx
	xorq	%r9, %rcx
	cmpq	%rcx, %rdx
	jne	.L17
	addq	$1, %r10
	addq	$8, %r12
	cmpq	8(%rsp), %r10
	jne	.L18
	movl	$.LC0, %edi
	call	puts
	xorl	%eax, %eax
	jmp	.L24
	.p2align 4,,10
	.p2align 3
.L28:
	movq	64(%rsp), %r8
	movq	40(%rsp), %rsi
	movq	16(%rsp), %rdx
	movq	24(%rsp), %rcx
	movq	32(%rsp), %r9
	jmp	.L6
.L17:
	movl	$.LC1, %edi
	call	puts
	movl	$1, %eax
.L24:
	addq	$152, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE18:
	.size	main, .-main
	.section	.rodata
	.align 32
	.type	vectors.1794, @object
	.size	vectors.1794, 512
vectors.1794:
	.byte	49
	.byte	14
	.byte	14
	.byte	-35
	.byte	71
	.byte	-37
	.byte	111
	.byte	114
	.byte	-3
	.byte	103
	.byte	-36
	.byte	-109
	.byte	-59
	.byte	57
	.byte	-8
	.byte	116
	.byte	90
	.byte	79
	.byte	-87
	.byte	-39
	.byte	9
	.byte	-128
	.byte	108
	.byte	13
	.byte	45
	.byte	126
	.byte	-5
	.byte	-41
	.byte	-106
	.byte	102
	.byte	103
	.byte	-123
	.byte	-73
	.byte	-121
	.byte	113
	.byte	39
	.byte	-32
	.byte	-108
	.byte	39
	.byte	-49
	.byte	-115
	.byte	-90
	.byte	-103
	.byte	-51
	.byte	100
	.byte	85
	.byte	118
	.byte	24
	.byte	-50
	.byte	-29
	.byte	-2
	.byte	88
	.byte	110
	.byte	70
	.byte	-55
	.byte	-53
	.byte	55
	.byte	-47
	.byte	1
	.byte	-117
	.byte	-11
	.byte	0
	.byte	2
	.byte	-85
	.byte	98
	.byte	36
	.byte	-109
	.byte	-102
	.byte	121
	.byte	-11
	.byte	-11
	.byte	-109
	.byte	-80
	.byte	-28
	.byte	-87
	.byte	11
	.byte	-33
	.byte	-126
	.byte	0
	.byte	-98
	.byte	-13
	.byte	-71
	.byte	-35
	.byte	-108
	.byte	-59
	.byte	-69
	.byte	93
	.byte	122
	.byte	-89
	.byte	-83
	.byte	107
	.byte	34
	.byte	70
	.byte	47
	.byte	-77
	.byte	-12
	.byte	-5
	.byte	-27
	.byte	14
	.byte	-122
	.byte	-68
	.byte	-113
	.byte	30
	.byte	117
	.byte	-112
	.byte	61
	.byte	-124
	.byte	-64
	.byte	39
	.byte	86
	.byte	-22
	.byte	20
	.byte	-18
	.byte	-14
	.byte	122
	.byte	-114
	.byte	-112
	.byte	-54
	.byte	35
	.byte	-9
	.byte	-27
	.byte	69
	.byte	-66
	.byte	73
	.byte	97
	.byte	-54
	.byte	41
	.byte	-95
	.byte	-37
	.byte	-101
	.byte	-62
	.byte	87
	.byte	127
	.byte	-52
	.byte	42
	.byte	63
	.byte	-108
	.byte	71
	.byte	-66
	.byte	44
	.byte	-11
	.byte	-23
	.byte	-102
	.byte	105
	.byte	-100
	.byte	-45
	.byte	-115
	.byte	-106
	.byte	-16
	.byte	-77
	.byte	-63
	.byte	75
	.byte	-67
	.byte	97
	.byte	121
	.byte	-89
	.byte	29
	.byte	-55
	.byte	109
	.byte	-69
	.byte	-104
	.byte	-18
	.byte	-94
	.byte	26
	.byte	-14
	.byte	92
	.byte	-42
	.byte	-66
	.byte	-57
	.byte	103
	.byte	59
	.byte	46
	.byte	-80
	.byte	-53
	.byte	-14
	.byte	-48
	.byte	-120
	.byte	62
	.byte	-93
	.byte	-29
	.byte	-107
	.byte	103
	.byte	83
	.byte	-109
	.byte	-56
	.byte	-50
	.byte	92
	.byte	-51
	.byte	-116
	.byte	3
	.byte	12
	.byte	-88
	.byte	-108
	.byte	-81
	.byte	73
	.byte	-10
	.byte	-58
	.byte	80
	.byte	-83
	.byte	-72
	.byte	-22
	.byte	-72
	.byte	-123
	.byte	-118
	.byte	-34
	.byte	-110
	.byte	-31
	.byte	-68
	.byte	-13
	.byte	21
	.byte	-69
	.byte	91
	.byte	-72
	.byte	53
	.byte	-40
	.byte	23
	.byte	-83
	.byte	-49
	.byte	107
	.byte	7
	.byte	99
	.byte	97
	.byte	46
	.byte	47
	.byte	-91
	.byte	-55
	.byte	29
	.byte	-89
	.byte	-84
	.byte	-86
	.byte	77
	.byte	-34
	.byte	113
	.byte	101
	.byte	-107
	.byte	-121
	.byte	102
	.byte	80
	.byte	-94
	.byte	-90
	.byte	40
	.byte	-17
	.byte	73
	.byte	92
	.byte	83
	.byte	-93
	.byte	-121
	.byte	-83
	.byte	66
	.byte	-61
	.byte	65
	.byte	-40
	.byte	-6
	.byte	-110
	.byte	-40
	.byte	50
	.byte	-50
	.byte	124
	.byte	-14
	.byte	114
	.byte	47
	.byte	81
	.byte	39
	.byte	113
	.byte	-29
	.byte	120
	.byte	89
	.byte	-7
	.byte	70
	.byte	35
	.byte	-13
	.byte	-89
	.byte	56
	.byte	18
	.byte	5
	.byte	-69
	.byte	26
	.byte	-80
	.byte	-32
	.byte	18
	.byte	-82
	.byte	-105
	.byte	-95
	.byte	15
	.byte	-44
	.byte	52
	.byte	-32
	.byte	21
	.byte	-76
	.byte	-93
	.byte	21
	.byte	8
	.byte	-66
	.byte	-1
	.byte	77
	.byte	49
	.byte	-127
	.byte	57
	.byte	98
	.byte	41
	.byte	-16
	.byte	-112
	.byte	121
	.byte	2
	.byte	77
	.byte	12
	.byte	-12
	.byte	-98
	.byte	-27
	.byte	-44
	.byte	-36
	.byte	-54
	.byte	92
	.byte	115
	.byte	51
	.byte	106
	.byte	118
	.byte	-40
	.byte	-65
	.byte	-102
	.byte	-48
	.byte	-89
	.byte	4
	.byte	83
	.byte	107
	.byte	-87
	.byte	62
	.byte	14
	.byte	-110
	.byte	89
	.byte	88
	.byte	-4
	.byte	-42
	.byte	66
	.byte	12
	.byte	-83
	.byte	-87
	.byte	21
	.byte	-62
	.byte	-101
	.byte	-56
	.byte	6
	.byte	115
	.byte	24
	.byte	-107
	.byte	43
	.byte	121
	.byte	-13
	.byte	-68
	.byte	10
	.byte	-90
	.byte	-44
	.byte	-14
	.byte	29
	.byte	-14
	.byte	-28
	.byte	29
	.byte	69
	.byte	53
	.byte	-7
	.byte	-121
	.byte	87
	.byte	117
	.byte	25
	.byte	4
	.byte	-113
	.byte	83
	.byte	-87
	.byte	16
	.byte	-91
	.byte	108
	.byte	-11
	.byte	-33
	.byte	-51
	.byte	-102
	.byte	-37
	.byte	-21
	.byte	117
	.byte	9
	.byte	92
	.byte	-51
	.byte	-104
	.byte	108
	.byte	-48
	.byte	81
	.byte	-87
	.byte	-53
	.byte	-98
	.byte	-53
	.byte	-93
	.byte	18
	.byte	-26
	.byte	-106
	.byte	-81
	.byte	-83
	.byte	-4
	.byte	44
	.byte	-26
	.byte	102
	.byte	-57
	.byte	114
	.byte	-2
	.byte	82
	.byte	-105
	.byte	90
	.byte	67
	.byte	100
	.byte	-18
	.byte	90
	.byte	22
	.byte	69
	.byte	-78
	.byte	118
	.byte	-43
	.byte	-110
	.byte	-95
	.byte	-78
	.byte	116
	.byte	-53
	.byte	-114
	.byte	-65
	.byte	-121
	.byte	-121
	.byte	10
	.byte	111
	.byte	-101
	.byte	-76
	.byte	32
	.byte	61
	.byte	-25
	.byte	-77
	.byte	-127
	.byte	-22
	.byte	-20
	.byte	-78
	.byte	-93
	.byte	11
	.byte	34
	.byte	-88
	.byte	127
	.byte	-103
	.byte	36
	.byte	-92
	.byte	60
	.byte	-63
	.byte	49
	.byte	87
	.byte	36
	.byte	-67
	.byte	-125
	.byte	-115
	.byte	58
	.byte	-81
	.byte	-65
	.byte	-115
	.byte	-73
	.byte	11
	.byte	26
	.byte	42
	.byte	50
	.byte	101
	.byte	-43
	.byte	26
	.byte	-22
	.byte	19
	.byte	80
	.byte	121
	.byte	-93
	.byte	35
	.byte	28
	.byte	-26
	.byte	96
	.byte	-109
	.byte	43
	.byte	40
	.byte	70
	.byte	-28
	.byte	-41
	.byte	6
	.byte	102
	.byte	-31
	.byte	-111
	.byte	95
	.byte	92
	.byte	-79
	.byte	-20
	.byte	-92
	.byte	108
	.byte	-13
	.byte	37
	.byte	-106
	.byte	92
	.byte	-95
	.byte	109
	.byte	98
	.byte	-97
	.byte	87
	.byte	95
	.byte	-14
	.byte	-114
	.byte	96
	.byte	56
	.byte	27
	.byte	-27
	.byte	114
	.byte	69
	.byte	6
	.byte	-21
	.byte	76
	.byte	50
	.byte	-118
	.byte	-107
	.ident	"GCC: (GNU) 4.7.2"
	.section	.note.GNU-stack,"",@progbits
