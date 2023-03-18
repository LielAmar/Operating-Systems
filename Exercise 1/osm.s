	.file	"osm.cpp"
# GNU C++14 (Ubuntu 9.4.0-1ubuntu1~20.04.1) version 9.4.0 (x86_64-linux-gnu)
#	compiled by GNU C version 9.4.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  -imultiarch x86_64-linux-gnu -D_GNU_SOURCE osm.cpp
# -mtune=generic -march=x86-64 -auxbase-strip osm.s -g -fverbose-asm
# -fasynchronous-unwind-tables -fstack-protector-strong -Wformat
# -Wformat-security -fstack-clash-protection -fcf-protection
# options enabled:  -fPIC -fPIE -faggressive-loop-optimizations
# -fassume-phsa -fasynchronous-unwind-tables -fauto-inc-dec -fcommon
# -fdelete-null-pointer-checks -fdwarf2-cfi-asm -fearly-inlining
# -feliminate-unused-debug-types -fexceptions -ffp-int-builtin-inexact
# -ffunction-cse -fgcse-lm -fgnu-runtime -fgnu-unique -fident
# -finline-atomics -fipa-stack-alignment -fira-hoist-pressure
# -fira-share-save-slots -fira-share-spill-slots -fivopts
# -fkeep-static-consts -fleading-underscore -flifetime-dse
# -flto-odr-type-merging -fmath-errno -fmerge-debug-strings -fpeephole
# -fplt -fprefetch-loop-arrays -freg-struct-return
# -fsched-critical-path-heuristic -fsched-dep-count-heuristic
# -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
# -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
# -fsched-stalled-insns-dep -fschedule-fusion -fsemantic-interposition
# -fshow-column -fshrink-wrap-separate -fsigned-zeros
# -fsplit-ivs-in-unroller -fssa-backprop -fstack-clash-protection
# -fstack-protector-strong -fstdarg-opt -fstrict-volatile-bitfields
# -fsync-libcalls -ftrapping-math -ftree-cselim -ftree-forwprop
# -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
# -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop
# -ftree-reassoc -ftree-scev-cprop -funit-at-a-time -funwind-tables
# -fverbose-asm -fzero-initialized-in-bss -m128bit-long-double -m64 -m80387
# -malign-stringops -mavx256-split-unaligned-load
# -mavx256-split-unaligned-store -mfancy-math-387 -mfp-ret-in-387 -mfxsr
# -mglibc -mieee-fp -mlong-double-80 -mmmx -mno-sse4 -mpush-args -mred-zone
# -msse -msse2 -mstv -mtls-direct-seg-refs -mvzeroupper

	.text
.Ltext0:
	.globl	_Z18osm_operation_timej
	.type	_Z18osm_operation_timej, @function
_Z18osm_operation_timej:
.LFB0:
	.file 1 "osm.cpp"
	.loc 1 4 52
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$64, %rsp	#,
	movl	%edi, -52(%rbp)	# iterations, iterations
# osm.cpp:4: double osm_operation_time(unsigned int iterations) {
	.loc 1 4 52
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp106
	movq	%rax, -8(%rbp)	# tmp106, D.2552
	xorl	%eax, %eax	# tmp106
# osm.cpp:5:     if (iterations <= 0) {
	.loc 1 5 5
	cmpl	$0, -52(%rbp)	#, iterations
	jne	.L2	#,
# osm.cpp:6:         return -1;
	.loc 1 6 17
	movsd	.LC0(%rip), %xmm0	#, _12
	jmp	.L10	#
.L2:
# osm.cpp:9:     struct timeval tv{};
	.loc 1 9 20
	movq	$0, -32(%rbp)	#, tv
	movq	$0, -24(%rbp)	#, tv
# osm.cpp:11:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 11 21
	leaq	-32(%rbp), %rax	#, tmp96
	movl	$0, %esi	#,
	movq	%rax, %rdi	# tmp96,
	call	gettimeofday@PLT	#
# osm.cpp:11:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 11 36
	cmpl	$-1, %eax	#, _1
	sete	%al	#, retval.0_18
# osm.cpp:11:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 11 5
	testb	%al, %al	# retval.0_18
	je	.L4	#,
# osm.cpp:12:         return -1;
	.loc 1 12 17
	movsd	.LC0(%rip), %xmm0	#, _12
	jmp	.L10	#
.L4:
# osm.cpp:15:     double startTime = (double) tv.tv_usec / 1000;
	.loc 1 15 36
	movq	-24(%rbp), %rax	# tv.tv_usec, _2
# osm.cpp:15:     double startTime = (double) tv.tv_usec / 1000;
	.loc 1 15 24
	cvtsi2sdq	%rax, %xmm0	# _2, _3
# osm.cpp:15:     double startTime = (double) tv.tv_usec / 1000;
	.loc 1 15 12
	movsd	.LC1(%rip), %xmm1	#, tmp98
	divsd	%xmm1, %xmm0	# tmp98, tmp97
	movsd	%xmm0, -40(%rbp)	# tmp97, startTime
.LBB2:
# osm.cpp:17:     for (int i = 0; i < iterations; i++) {
	.loc 1 17 14
	movl	$0, -44(%rbp)	#, i
.L6:
# osm.cpp:17:     for (int i = 0; i < iterations; i++) {
	.loc 1 17 21 discriminator 3
	movl	-44(%rbp), %eax	# i, i.1_4
# osm.cpp:17:     for (int i = 0; i < iterations; i++) {
	.loc 1 17 23 discriminator 3
	cmpl	%eax, -52(%rbp)	# i.1_4, iterations
	jbe	.L5	#,
# osm.cpp:17:     for (int i = 0; i < iterations; i++) {
	.loc 1 17 5 discriminator 2
	addl	$1, -44(%rbp)	#, i
	jmp	.L6	#
.L5:
.LBE2:
# osm.cpp:21:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 21 21
	leaq	-32(%rbp), %rax	#, tmp99
	movl	$0, %esi	#,
	movq	%rax, %rdi	# tmp99,
	call	gettimeofday@PLT	#
# osm.cpp:21:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 21 36
	cmpl	$-1, %eax	#, _5
	sete	%al	#, retval.2_23
# osm.cpp:21:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 21 5
	testb	%al, %al	# retval.2_23
	je	.L7	#,
# osm.cpp:22:         return -1;
	.loc 1 22 17
	movsd	.LC0(%rip), %xmm0	#, _12
	jmp	.L10	#
.L7:
# osm.cpp:25:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 25 26
	movq	-24(%rbp), %rax	# tv.tv_usec, _6
# osm.cpp:25:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 25 14
	cvtsi2sdq	%rax, %xmm0	# _6, _7
# osm.cpp:25:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 25 34
	movsd	.LC1(%rip), %xmm1	#, tmp100
	divsd	%xmm1, %xmm0	# tmp100, _8
# osm.cpp:25:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 25 42
	movapd	%xmm0, %xmm1	# _8, _8
	subsd	-40(%rbp), %xmm1	# startTime, _8
# osm.cpp:25:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 25 55
	movl	-52(%rbp), %eax	# iterations, tmp101
	testq	%rax, %rax	# tmp101
	js	.L8	#,
	cvtsi2sdq	%rax, %xmm0	# tmp101, _10
	jmp	.L9	#
.L8:
	movq	%rax, %rdx	# tmp101, tmp103
	shrq	%rdx	# tmp103
	andl	$1, %eax	#, tmp104
	orq	%rax, %rdx	# tmp104, tmp103
	cvtsi2sdq	%rdx, %xmm0	# tmp103, tmp102
	addsd	%xmm0, %xmm0	# tmp102, _10
.L9:
# osm.cpp:25:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 25 57
	divsd	%xmm0, %xmm1	# _10, _9
	movapd	%xmm1, %xmm0	# _9, _12
.L10:
# osm.cpp:26: }
	.loc 1 26 1 discriminator 1
	movq	-8(%rbp), %rax	# D.2552, tmp107
	xorq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp107
	je	.L11	#,
# osm.cpp:26: }
	.loc 1 26 1 is_stmt 0
	call	__stack_chk_fail@PLT	#
.L11:
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE0:
	.size	_Z18osm_operation_timej, .-_Z18osm_operation_timej
	.globl	_Z10empty_funcv
	.type	_Z10empty_funcv, @function
_Z10empty_funcv:
.LFB1:
	.loc 1 28 19 is_stmt 1
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
# osm.cpp:28: void empty_func() {}
	.loc 1 28 20
	nop	
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE1:
	.size	_Z10empty_funcv, .-_Z10empty_funcv
	.globl	_Z17osm_function_timej
	.type	_Z17osm_function_timej, @function
_Z17osm_function_timej:
.LFB2:
	.loc 1 30 51
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$64, %rsp	#,
	movl	%edi, -52(%rbp)	# iterations, iterations
# osm.cpp:30: double osm_function_time(unsigned int iterations) {
	.loc 1 30 51
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp106
	movq	%rax, -8(%rbp)	# tmp106, D.2554
	xorl	%eax, %eax	# tmp106
# osm.cpp:31:     if (iterations <= 0) {
	.loc 1 31 5
	cmpl	$0, -52(%rbp)	#, iterations
	jne	.L14	#,
# osm.cpp:32:         return -1;
	.loc 1 32 17
	movsd	.LC0(%rip), %xmm0	#, _12
	jmp	.L22	#
.L14:
# osm.cpp:35:     struct timeval tv{};
	.loc 1 35 20
	movq	$0, -32(%rbp)	#, tv
	movq	$0, -24(%rbp)	#, tv
# osm.cpp:37:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 37 21
	leaq	-32(%rbp), %rax	#, tmp96
	movl	$0, %esi	#,
	movq	%rax, %rdi	# tmp96,
	call	gettimeofday@PLT	#
# osm.cpp:37:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 37 36
	cmpl	$-1, %eax	#, _1
	sete	%al	#, retval.3_19
# osm.cpp:37:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 37 5
	testb	%al, %al	# retval.3_19
	je	.L16	#,
# osm.cpp:38:         return -1;
	.loc 1 38 17
	movsd	.LC0(%rip), %xmm0	#, _12
	jmp	.L22	#
.L16:
# osm.cpp:41:     double startTime = (double) tv.tv_usec / 1000;
	.loc 1 41 36
	movq	-24(%rbp), %rax	# tv.tv_usec, _2
# osm.cpp:41:     double startTime = (double) tv.tv_usec / 1000;
	.loc 1 41 24
	cvtsi2sdq	%rax, %xmm0	# _2, _3
# osm.cpp:41:     double startTime = (double) tv.tv_usec / 1000;
	.loc 1 41 12
	movsd	.LC1(%rip), %xmm1	#, tmp98
	divsd	%xmm1, %xmm0	# tmp98, tmp97
	movsd	%xmm0, -40(%rbp)	# tmp97, startTime
.LBB3:
# osm.cpp:43:     for (int i = 0; i < iterations; i++) {
	.loc 1 43 14
	movl	$0, -44(%rbp)	#, i
.L18:
# osm.cpp:43:     for (int i = 0; i < iterations; i++) {
	.loc 1 43 21 discriminator 3
	movl	-44(%rbp), %eax	# i, i.4_4
# osm.cpp:43:     for (int i = 0; i < iterations; i++) {
	.loc 1 43 23 discriminator 3
	cmpl	%eax, -52(%rbp)	# i.4_4, iterations
	jbe	.L17	#,
# osm.cpp:44:         empty_func();
	.loc 1 44 19 discriminator 2
	call	_Z10empty_funcv	#
# osm.cpp:43:     for (int i = 0; i < iterations; i++) {
	.loc 1 43 5 discriminator 2
	addl	$1, -44(%rbp)	#, i
	jmp	.L18	#
.L17:
.LBE3:
# osm.cpp:47:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 47 21
	leaq	-32(%rbp), %rax	#, tmp99
	movl	$0, %esi	#,
	movq	%rax, %rdi	# tmp99,
	call	gettimeofday@PLT	#
# osm.cpp:47:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 47 36
	cmpl	$-1, %eax	#, _5
	sete	%al	#, retval.5_25
# osm.cpp:47:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 47 5
	testb	%al, %al	# retval.5_25
	je	.L19	#,
# osm.cpp:48:         return -1;
	.loc 1 48 17
	movsd	.LC0(%rip), %xmm0	#, _12
	jmp	.L22	#
.L19:
# osm.cpp:51:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 51 26
	movq	-24(%rbp), %rax	# tv.tv_usec, _6
# osm.cpp:51:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 51 14
	cvtsi2sdq	%rax, %xmm0	# _6, _7
# osm.cpp:51:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 51 34
	movsd	.LC1(%rip), %xmm1	#, tmp100
	divsd	%xmm1, %xmm0	# tmp100, _8
# osm.cpp:51:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 51 42
	movapd	%xmm0, %xmm1	# _8, _8
	subsd	-40(%rbp), %xmm1	# startTime, _8
# osm.cpp:51:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 51 55
	movl	-52(%rbp), %eax	# iterations, tmp101
	testq	%rax, %rax	# tmp101
	js	.L20	#,
	cvtsi2sdq	%rax, %xmm0	# tmp101, _10
	jmp	.L21	#
.L20:
	movq	%rax, %rdx	# tmp101, tmp103
	shrq	%rdx	# tmp103
	andl	$1, %eax	#, tmp104
	orq	%rax, %rdx	# tmp104, tmp103
	cvtsi2sdq	%rdx, %xmm0	# tmp103, tmp102
	addsd	%xmm0, %xmm0	# tmp102, _10
.L21:
# osm.cpp:51:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 51 57
	divsd	%xmm0, %xmm1	# _10, _9
	movapd	%xmm1, %xmm0	# _9, _12
.L22:
# osm.cpp:52: }
	.loc 1 52 1 discriminator 1
	movq	-8(%rbp), %rax	# D.2554, tmp107
	xorq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp107
	je	.L23	#,
# osm.cpp:52: }
	.loc 1 52 1 is_stmt 0
	call	__stack_chk_fail@PLT	#
.L23:
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE2:
	.size	_Z17osm_function_timej, .-_Z17osm_function_timej
	.globl	_Z16osm_syscall_timej
	.type	_Z16osm_syscall_timej, @function
_Z16osm_syscall_timej:
.LFB3:
	.loc 1 57 50 is_stmt 1
	.cfi_startproc
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	pushq	%rbx	#
	subq	$72, %rsp	#,
	.cfi_offset 3, -24
	movl	%edi, -68(%rbp)	# iterations, iterations
# osm.cpp:57: double osm_syscall_time(unsigned int iterations) {
	.loc 1 57 50
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp110
	movq	%rax, -24(%rbp)	# tmp110, D.2555
	xorl	%eax, %eax	# tmp110
# osm.cpp:58:     if (iterations <= 0) {
	.loc 1 58 5
	cmpl	$0, -68(%rbp)	#, iterations
	jne	.L25	#,
# osm.cpp:59:         return -1;
	.loc 1 59 17
	movsd	.LC0(%rip), %xmm0	#, _12
	jmp	.L33	#
.L25:
# osm.cpp:62:     struct timeval tv{};
	.loc 1 62 20
	movq	$0, -48(%rbp)	#, tv
	movq	$0, -40(%rbp)	#, tv
# osm.cpp:64:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 64 21
	leaq	-48(%rbp), %rax	#, tmp96
	movl	$0, %esi	#,
	movq	%rax, %rdi	# tmp96,
	call	gettimeofday@PLT	#
# osm.cpp:64:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 64 36
	cmpl	$-1, %eax	#, _1
	sete	%al	#, retval.6_18
# osm.cpp:64:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 64 5
	testb	%al, %al	# retval.6_18
	je	.L27	#,
# osm.cpp:65:         return -1;
	.loc 1 65 17
	movsd	.LC0(%rip), %xmm0	#, _12
	jmp	.L33	#
.L27:
# osm.cpp:68:     double startTime = (double) tv.tv_usec / 1000;
	.loc 1 68 36
	movq	-40(%rbp), %rax	# tv.tv_usec, _2
# osm.cpp:68:     double startTime = (double) tv.tv_usec / 1000;
	.loc 1 68 24
	cvtsi2sdq	%rax, %xmm0	# _2, _3
# osm.cpp:68:     double startTime = (double) tv.tv_usec / 1000;
	.loc 1 68 12
	movsd	.LC1(%rip), %xmm1	#, tmp98
	divsd	%xmm1, %xmm0	# tmp98, tmp97
	movsd	%xmm0, -56(%rbp)	# tmp97, startTime
.LBB4:
# osm.cpp:70:     for (int i = 0; i < iterations; i++) {
	.loc 1 70 14
	movl	$0, -60(%rbp)	#, i
.L29:
# osm.cpp:70:     for (int i = 0; i < iterations; i++) {
	.loc 1 70 21 discriminator 3
	movl	-60(%rbp), %eax	# i, i.7_4
# osm.cpp:70:     for (int i = 0; i < iterations; i++) {
	.loc 1 70 23 discriminator 3
	cmpl	%eax, -68(%rbp)	# i.7_4, iterations
	jbe	.L28	#,
# osm.cpp:71:         OSM_NULLSYSCALL;
	.loc 1 71 24 discriminator 2
	movl	$-1, %eax	#, tmp99
	movl	$0, %esi	#, tmp100
	movl	$0, %ecx	#, tmp101
	movl	$0, %edx	#, tmp102
	movl	%esi, %ebx	# tmp100, tmp100
#APP
# 71 "osm.cpp" 1
	int $0x80 
# 0 "" 2
# osm.cpp:70:     for (int i = 0; i < iterations; i++) {
	.loc 1 70 5 discriminator 2
#NO_APP
	addl	$1, -60(%rbp)	#, i
	jmp	.L29	#
.L28:
.LBE4:
# osm.cpp:74:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 74 21
	leaq	-48(%rbp), %rax	#, tmp103
	movl	$0, %esi	#,
	movq	%rax, %rdi	# tmp103,
	call	gettimeofday@PLT	#
# osm.cpp:74:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 74 36
	cmpl	$-1, %eax	#, _5
	sete	%al	#, retval.8_23
# osm.cpp:74:     if (gettimeofday(&tv, nullptr) == -1) {
	.loc 1 74 5
	testb	%al, %al	# retval.8_23
	je	.L30	#,
# osm.cpp:75:         return -1;
	.loc 1 75 17
	movsd	.LC0(%rip), %xmm0	#, _12
	jmp	.L33	#
.L30:
# osm.cpp:78:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 78 26
	movq	-40(%rbp), %rax	# tv.tv_usec, _6
# osm.cpp:78:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 78 14
	cvtsi2sdq	%rax, %xmm0	# _6, _7
# osm.cpp:78:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 78 34
	movsd	.LC1(%rip), %xmm1	#, tmp104
	divsd	%xmm1, %xmm0	# tmp104, _8
# osm.cpp:78:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 78 42
	movapd	%xmm0, %xmm1	# _8, _8
	subsd	-56(%rbp), %xmm1	# startTime, _8
# osm.cpp:78:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 78 55
	movl	-68(%rbp), %eax	# iterations, tmp105
	testq	%rax, %rax	# tmp105
	js	.L31	#,
	cvtsi2sdq	%rax, %xmm0	# tmp105, _10
	jmp	.L32	#
.L31:
	movq	%rax, %rdx	# tmp105, tmp107
	shrq	%rdx	# tmp107
	andl	$1, %eax	#, tmp108
	orq	%rax, %rdx	# tmp108, tmp107
	cvtsi2sdq	%rdx, %xmm0	# tmp107, tmp106
	addsd	%xmm0, %xmm0	# tmp106, _10
.L32:
# osm.cpp:78:     return (((double) tv.tv_usec / 1000) - startTime) / iterations;
	.loc 1 78 57
	divsd	%xmm0, %xmm1	# _10, _9
	movapd	%xmm1, %xmm0	# _9, _12
.L33:
# osm.cpp:79: }
	.loc 1 79 1 discriminator 1
	movq	-24(%rbp), %rax	# D.2555, tmp111
	xorq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp111
	je	.L34	#,
# osm.cpp:79: }
	.loc 1 79 1 is_stmt 0
	call	__stack_chk_fail@PLT	#
.L34:
	addq	$72, %rsp	#,
	popq	%rbx	#
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE3:
	.size	_Z16osm_syscall_timej, .-_Z16osm_syscall_timej
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	-1074790400
	.align 8
.LC1:
	.long	0
	.long	1083129856
	.text
.Letext0:
	.file 2 "/usr/include/x86_64-linux-gnu/bits/types.h"
	.file 3 "/usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x225
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF19
	.byte	0x4
	.long	.LASF20
	.long	.LASF21
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF0
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF1
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x4
	.long	.LASF7
	.byte	0x2
	.byte	0xa0
	.byte	0x1a
	.long	0x5e
	.uleb128 0x4
	.long	.LASF8
	.byte	0x2
	.byte	0xa2
	.byte	0x1f
	.long	0x5e
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF9
	.uleb128 0x5
	.long	.LASF22
	.byte	0x10
	.byte	0x3
	.byte	0x8
	.byte	0x8
	.long	0xac
	.uleb128 0x6
	.long	.LASF10
	.byte	0x3
	.byte	0xa
	.byte	0xc
	.long	0x65
	.byte	0
	.uleb128 0x6
	.long	.LASF11
	.byte	0x3
	.byte	0xb
	.byte	0x11
	.long	0x71
	.byte	0x8
	.byte	0
	.uleb128 0x7
	.long	.LASF14
	.byte	0x1
	.byte	0x39
	.byte	0x8
	.long	.LASF16
	.long	0x121
	.quad	.LFB3
	.quad	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.long	0x121
	.uleb128 0x8
	.long	.LASF18
	.byte	0x1
	.byte	0x39
	.byte	0x26
	.long	0x3b
	.uleb128 0x3
	.byte	0x91
	.sleb128 -84
	.uleb128 0x9
	.string	"tv"
	.byte	0x1
	.byte	0x3e
	.byte	0x14
	.long	0x84
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0xa
	.long	.LASF12
	.byte	0x1
	.byte	0x44
	.byte	0xc
	.long	0x121
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0xb
	.quad	.LBB4
	.quad	.LBE4-.LBB4
	.uleb128 0x9
	.string	"i"
	.byte	0x1
	.byte	0x46
	.byte	0xe
	.long	0x57
	.uleb128 0x3
	.byte	0x91
	.sleb128 -76
	.byte	0
	.byte	0
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.long	.LASF13
	.uleb128 0x7
	.long	.LASF15
	.byte	0x1
	.byte	0x1e
	.byte	0x8
	.long	.LASF17
	.long	0x121
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0x19b
	.uleb128 0x8
	.long	.LASF18
	.byte	0x1
	.byte	0x1e
	.byte	0x27
	.long	0x3b
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x9
	.string	"tv"
	.byte	0x1
	.byte	0x23
	.byte	0x14
	.long	0x84
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0xa
	.long	.LASF12
	.byte	0x1
	.byte	0x29
	.byte	0xc
	.long	0x121
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xb
	.quad	.LBB3
	.quad	.LBE3-.LBB3
	.uleb128 0x9
	.string	"i"
	.byte	0x1
	.byte	0x2b
	.byte	0xe
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.byte	0
	.byte	0
	.uleb128 0xc
	.long	.LASF23
	.byte	0x1
	.byte	0x1c
	.byte	0x6
	.long	.LASF24
	.quad	.LFB1
	.quad	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0xd
	.long	.LASF25
	.byte	0x1
	.byte	0x4
	.byte	0x8
	.long	.LASF26
	.long	0x121
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x8
	.long	.LASF18
	.byte	0x1
	.byte	0x4
	.byte	0x28
	.long	0x3b
	.uleb128 0x3
	.byte	0x91
	.sleb128 -68
	.uleb128 0x9
	.string	"tv"
	.byte	0x1
	.byte	0x9
	.byte	0x14
	.long	0x84
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0xa
	.long	.LASF12
	.byte	0x1
	.byte	0xf
	.byte	0xc
	.long	0x121
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xb
	.quad	.LBB2
	.quad	.LBE2-.LBB2
	.uleb128 0x9
	.string	"i"
	.byte	0x1
	.byte	0x11
	.byte	0xe
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF25:
	.string	"osm_operation_time"
.LASF2:
	.string	"unsigned int"
.LASF23:
	.string	"empty_func"
.LASF14:
	.string	"osm_syscall_time"
.LASF24:
	.string	"_Z10empty_funcv"
.LASF16:
	.string	"_Z16osm_syscall_timej"
.LASF21:
	.string	"/mnt/d/Workspace/C/OS/Exercise 1"
.LASF26:
	.string	"_Z18osm_operation_timej"
.LASF18:
	.string	"iterations"
.LASF0:
	.string	"unsigned char"
.LASF12:
	.string	"startTime"
.LASF19:
	.string	"GNU C++14 9.4.0 -mtune=generic -march=x86-64 -g -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection"
.LASF9:
	.string	"char"
.LASF22:
	.string	"timeval"
.LASF6:
	.string	"long int"
.LASF8:
	.string	"__suseconds_t"
.LASF3:
	.string	"long unsigned int"
.LASF13:
	.string	"double"
.LASF11:
	.string	"tv_usec"
.LASF7:
	.string	"__time_t"
.LASF10:
	.string	"tv_sec"
.LASF1:
	.string	"short unsigned int"
.LASF4:
	.string	"signed char"
.LASF15:
	.string	"osm_function_time"
.LASF5:
	.string	"short int"
.LASF20:
	.string	"osm.cpp"
.LASF17:
	.string	"_Z17osm_function_timej"
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
