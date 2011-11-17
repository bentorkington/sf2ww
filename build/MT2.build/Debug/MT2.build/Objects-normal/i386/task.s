	.section __DWARF,__debug_frame,regular,debug
Lsection__debug_frame:
	.section __DWARF,__debug_info,regular,debug
Lsection__debug_info:
	.section __DWARF,__debug_abbrev,regular,debug
Lsection__debug_abbrev:
	.section __DWARF,__debug_aranges,regular,debug
Lsection__debug_aranges:
	.section __DWARF,__debug_macinfo,regular,debug
Lsection__debug_macinfo:
	.section __DWARF,__debug_line,regular,debug
Lsection__debug_line:
	.section __DWARF,__debug_loc,regular,debug
Lsection__debug_loc:
	.section __DWARF,__debug_pubnames,regular,debug
Lsection__debug_pubnames:
	.section __DWARF,__debug_pubtypes,regular,debug
Lsection__debug_pubtypes:
	.section __DWARF,__debug_inlined,regular,debug
Lsection__debug_inlined:
	.section __DWARF,__debug_str,regular,debug
Lsection__debug_str:
	.section __DWARF,__debug_ranges,regular,debug
Lsection__debug_ranges:
	.section __DWARF,__debug_abbrev,regular,debug
Ldebug_abbrev0:
	.section __DWARF,__debug_info,regular,debug
Ldebug_info0:
	.section __DWARF,__debug_line,regular,debug
Ldebug_line0:
	.text
Ltext0:
.globl _task_timer
_task_timer:
LFB3:
	.file 1 "/Users/ben/Documents/dev/MT2/FistBlue/task.c"
	.loc 1 61 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI0:
	movl	%esp, %ebp
LCFI1:
	pushl	%edi
LCFI2:
	pushl	%esi
LCFI3:
	pushl	%ebx
LCFI4:
	subl	$44, %esp
LCFI5:
	call	L9
"L00000000001$pb":
L9:
	popl	%ebx
	.loc 1 64 0
	leal	L_gtimercount$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	leal	1(%eax), %edx
	leal	L_gtimercount$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	.loc 1 66 0
	movl	$0, -28(%ebp)
	jmp	L2
L3:
	.loc 1 67 0
	movl	-28(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movzbl	(%edx,%eax), %eax
	cmpb	$1, %al
	jne	L4
	.loc 1 68 0
	movl	-28(%ebp), %ecx
	leal	L_Tasks$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %ecx, %edx
	movzbl	1(%edx,%eax), %eax
	leal	-1(%eax), %esi
	leal	L_Tasks$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %ecx, %edx
	movl	%edx, -44(%ebp)
	movl	%esi, %edx
	movl	-44(%ebp), %edi
	movb	%dl, 1(%edi,%eax)
	leal	L_Tasks$non_lazy_ptr-"L00000000001$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %ecx, %edx
	movzbl	1(%edx,%eax), %eax
	testb	%al, %al
	jne	L4
	.loc 1 69 0
	movl	-28(%ebp), %eax
	leal	L_Tasks$non_lazy_ptr-"L00000000001$pb"(%ebx), %edx
	movl	(%edx), %edx
	imull	$52, %eax, %eax
	movb	$4, (%eax,%edx)
L4:
	.loc 1 66 0
	incl	-28(%ebp)
L2:
	cmpl	$15, -28(%ebp)
	jle	L3
	.loc 1 73 0
	call	_despatch_tasks
	.loc 1 74 0
	addl	$44, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
	leave
	ret
LFE3:
	.cstring
LC0:
	.ascii "Task Table:\0"
	.align 2
LC1:
	.ascii "Task %2d: stat %02x code %08x\12\0"
	.text
.globl _printtasktable
_printtasktable:
LFB4:
	.loc 1 76 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI6:
	movl	%esp, %ebp
LCFI7:
	pushl	%ebx
LCFI8:
	subl	$36, %esp
LCFI9:
	call	L15
"L00000000002$pb":
L15:
	popl	%ebx
	.loc 1 79 0
	leal	LC0-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	.loc 1 80 0
	movl	$0, -12(%ebp)
	jmp	L11
L12:
	.loc 1 81 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	addl	$20, %eax
	movl	(%eax), %ecx
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000002$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movzbl	(%edx,%eax), %eax
	movzbl	%al, %eax
	movl	%ecx, 12(%esp)
	movl	%eax, 8(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC1-"L00000000002$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	.loc 1 80 0
	incl	-12(%ebp)
L11:
	cmpl	$15, -12(%ebp)
	jle	L12
	.loc 1 83 0
	addl	$36, %esp
	popl	%ebx
	leave
	ret
LFE4:
.globl _justdie
_justdie:
LFB5:
	.loc 1 85 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI10:
	movl	%esp, %ebp
LCFI11:
	pushl	%ebx
LCFI12:
	subl	$36, %esp
LCFI13:
	call	L18
"L00000000003$pb":
L18:
	popl	%ebx
	.loc 1 86 0
	leal	L_CurrentTask$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	imull	$52, %eax, %eax
	movl	%eax, %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000003$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -12(%ebp)
	.loc 1 87 0
	movl	-12(%ebp), %eax
	addl	$40, %eax
	movl	$8, 4(%esp)
	movl	%eax, (%esp)
	call	_memclear
	.loc 1 88 0
	movl	-12(%ebp), %eax
	movb	$0, (%eax)
	.loc 1 89 0
	movl	-12(%ebp), %eax
	movb	$0, 2(%eax)
	.loc 1 90 0
	addl	$36, %esp
	popl	%ebx
	leave
	ret
LFE5:
.globl _diefree
_diefree:
LFB6:
	.loc 1 93 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI14:
	movl	%esp, %ebp
LCFI15:
	pushl	%ebx
LCFI16:
	subl	$36, %esp
LCFI17:
	call	L21
"L00000000004$pb":
L21:
	popl	%ebx
	.loc 1 94 0
	leal	L_CurrentTask$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	imull	$52, %eax, %eax
	movl	%eax, %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, -12(%ebp)
	.loc 1 95 0
	movl	-12(%ebp), %eax
	addl	$40, %eax
	movl	$8, 4(%esp)
	movl	%eax, (%esp)
	call	_memclear
	.loc 1 96 0
	movl	-12(%ebp), %eax
	movb	$0, (%eax)
	.loc 1 97 0
	movl	-12(%ebp), %eax
	movb	$0, 2(%eax)
	.loc 1 98 0
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	leal	-4(%eax), %edx
	movl	-12(%ebp), %eax
	movl	%eax, (%edx)
	.loc 1 99 0
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	leal	-4(%eax), %edx
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	.loc 1 100 0
	leal	L_FreeTasks$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzwl	(%eax), %eax
	leal	1(%eax), %edx
	leal	L_FreeTasks$non_lazy_ptr-"L00000000004$pb"(%ebx), %eax
	movl	(%eax), %eax
	movw	%dx, (%eax)
	.loc 1 102 0
	addl	$36, %esp
	popl	%ebx
	leave
	ret
LFE6:
	.cstring
LC2:
	.ascii "Effect\0"
LC3:
	.ascii "-----\0"
LC4:
	.ascii "trap7 Failed!\0"
	.text
.globl _wrap_trap7
_wrap_trap7:
LFB7:
	.loc 1 105 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI18:
	movl	%esp, %ebp
LCFI19:
	pushl	%ebx
LCFI20:
	subl	$52, %esp
LCFI21:
	call	L27
"L00000000005$pb":
L27:
	popl	%ebx
	movl	12(%ebp), %eax
	movl	16(%ebp), %edx
	movw	%ax, -28(%ebp)
	movw	%dx, -32(%ebp)
	.loc 1 108 0
	leal	L_FreeTasks$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzwl	(%eax), %eax
	testw	%ax, %ax
	je	L23
	.loc 1 109 0
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
	.loc 1 110 0
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	leal	4(%eax), %edx
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	.loc 1 111 0
	leal	L_FreeTasks$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzwl	(%eax), %eax
	leal	-1(%eax), %edx
	leal	L_FreeTasks$non_lazy_ptr-"L00000000005$pb"(%ebx), %eax
	movl	(%eax), %eax
	movw	%dx, (%eax)
	.loc 1 112 0
	movl	-12(%ebp), %eax
	movb	$12, (%eax)
	.loc 1 113 0
	movl	8(%ebp), %edx
	movl	-12(%ebp), %eax
	movl	%edx, 20(%eax)
	.loc 1 114 0
	movl	-12(%ebp), %edx
	movzwl	-28(%ebp), %eax
	movw	%ax, 40(%edx)
	.loc 1 115 0
	movzwl	-32(%ebp), %eax
	shrw	$8, %ax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	movb	%dl, 42(%eax)
	.loc 1 116 0
	movzwl	-32(%ebp), %eax
	movl	%eax, %edx
	movl	-12(%ebp), %eax
	movb	%dl, 43(%eax)
	.loc 1 118 0
	movl	-12(%ebp), %edx
	leal	LC2-"L00000000005$pb"(%ebx), %eax
	movl	%eax, 24(%edx)
	.loc 1 119 0
	movl	-12(%ebp), %edx
	leal	LC3-"L00000000005$pb"(%ebx), %eax
	movl	%eax, 28(%edx)
	.loc 1 120 0
	movl	-12(%ebp), %eax
	movzbl	48(%eax), %eax
	movsbl	%al,%eax
	movl	%eax, (%esp)
	call	_RHCreateThread
	.loc 1 122 0
	movl	-12(%ebp), %eax
	movb	$0, 2(%eax)
	jmp	L26
L23:
	.loc 1 124 0
	leal	LC4-"L00000000005$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	.loc 1 125 0
	call	_printtasktable
L26:
	.loc 1 128 0
	addl	$52, %esp
	popl	%ebx
	leave
	ret
LFE7:
	.cstring
LC5:
	.ascii "%2d %20s \0"
LC6:
	.ascii "TASK_SLEEP \0"
LC7:
	.ascii "TASK_READY \0"
LC8:
	.ascii "?????????? \0"
LC9:
	.ascii "%20s\12\0"
	.text
.globl _print_task_table
_print_task_table:
LFB8:
	.loc 1 131 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI22:
	movl	%esp, %ebp
LCFI23:
	pushl	%ebx
LCFI24:
	subl	$52, %esp
LCFI25:
	call	L39
"L00000000006$pb":
L39:
	popl	%ebx
LBB2:
	.loc 1 132 0
	movl	$0, -12(%ebp)
	jmp	L29
L30:
	.loc 1 133 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movzbl	(%edx,%eax), %eax
	testb	%al, %al
	je	L31
	.loc 1 134 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	addl	$24, %eax
	movl	(%eax), %eax
	movl	%eax, 8(%esp)
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC5-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	.loc 1 135 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movzbl	(%edx,%eax), %eax
	movzbl	%al, %eax
	movl	%eax, -28(%ebp)
	cmpl	$1, -28(%ebp)
	je	L34
	cmpl	$4, -28(%ebp)
	je	L35
	jmp	L33
L34:
	.loc 1 137 0
	leal	LC6-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	.loc 1 138 0
	jmp	L36
L35:
	.loc 1 140 0
	leal	LC7-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	.loc 1 141 0
	jmp	L36
L33:
	.loc 1 143 0
	leal	LC8-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
L36:
	.loc 1 146 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000006$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	addl	$28, %eax
	movl	(%eax), %eax
	movl	%eax, 4(%esp)
	leal	LC9-"L00000000006$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
L31:
	.loc 1 132 0
	incl	-12(%ebp)
L29:
	cmpl	$15, -12(%ebp)
	jle	L30
LBE2:
	.loc 1 149 0
	addl	$52, %esp
	popl	%ebx
	leave
	ret
LFE8:
.globl _sf2sleep
_sf2sleep:
LFB9:
	.loc 1 152 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI26:
	movl	%esp, %ebp
LCFI27:
	pushl	%ebx
LCFI28:
	subl	$20, %esp
LCFI29:
	call	L45
"L00000000007$pb":
L45:
	popl	%ebx
	.loc 1 153 0
	cmpl	$0, 8(%ebp)
	je	L41
	.loc 1 154 0
	leal	L_CurrentTask$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%edx
	movl	8(%ebp), %eax
	movl	%eax, %ecx
	leal	L_Tasks$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movb	%cl, 1(%edx,%eax)
	.loc 1 155 0
	leal	L_CurrentTask$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%edx
	leal	L_Tasks$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movb	$1, (%edx,%eax)
	jmp	L43
L41:
	.loc 1 157 0
	leal	L_CurrentTask$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%edx
	leal	L_Tasks$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movb	$1, (%edx,%eax)
L43:
	.loc 1 178 0
	leal	L_CurrentTask$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	imull	$52, %eax, %eax
	movl	%eax, %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000007$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, (%esp)
	call	_RHWait
	.loc 1 179 0
	addl	$20, %esp
	popl	%ebx
	leave
	ret
LFE9:
.globl _exit_ready
_exit_ready:
LFB10:
	.loc 1 195 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI30:
	movl	%esp, %ebp
LCFI31:
	pushl	%ebx
LCFI32:
	subl	$20, %esp
LCFI33:
	call	L48
"L00000000008$pb":
L48:
	popl	%ebx
	.loc 1 196 0
	leal	L_CurrentTask$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%edx
	leal	L_Tasks$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movb	$4, (%edx,%eax)
	.loc 1 203 0
	leal	L_CurrentTask$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	imull	$52, %eax, %eax
	movl	%eax, %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000008$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, (%esp)
	call	_RHWait
	.loc 1 205 0
	addl	$20, %esp
	popl	%ebx
	leave
	ret
LFE10:
.globl _task_die
_task_die:
LFB11:
	.loc 1 208 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI34:
	movl	%esp, %ebp
LCFI35:
	pushl	%ebx
LCFI36:
	subl	$20, %esp
LCFI37:
	call	L51
"L00000000009$pb":
L51:
	popl	%ebx
	.loc 1 209 0
	leal	L_CurrentTask$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%edx
	leal	L_Tasks$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movb	$0, (%edx,%eax)
	.loc 1 217 0
	leal	L_CurrentTask$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzbl	(%eax), %eax
	movsbl	%al,%eax
	imull	$52, %eax, %eax
	movl	%eax, %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000009$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, (%esp)
	call	_RHExit
	.loc 1 219 0
	addl	$20, %esp
	popl	%ebx
	leave
	ret
LFE11:
	.cstring
LC10:
	.ascii "Task_kill %d\12\0"
	.text
.globl _task_kill
_task_kill:
LFB12:
	.loc 1 221 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI38:
	movl	%esp, %ebp
LCFI39:
	pushl	%ebx
LCFI40:
	subl	$36, %esp
LCFI41:
	call	L54
"L00000000010$pb":
L54:
	popl	%ebx
	movl	8(%ebp), %eax
	movw	%ax, -12(%ebp)
	.loc 1 222 0
	movzwl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC10-"L00000000010$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	.loc 1 223 0
	movzwl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000010$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movb	$0, (%edx,%eax)
	.loc 1 225 0
	movzwl	-12(%ebp), %eax
	imull	$52, %eax, %eax
	movl	%eax, %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000010$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, (%esp)
	call	_RHKill
	.loc 1 228 0
	addl	$36, %esp
	popl	%ebx
	leave
	ret
LFE12:
	.cstring
LC11:
	.ascii "die_top8()\0"
	.text
.globl _die_top8
_die_top8:
LFB13:
	.loc 1 230 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI42:
	movl	%esp, %ebp
LCFI43:
	pushl	%ebx
LCFI44:
	subl	$36, %esp
LCFI45:
	call	L62
"L00000000011$pb":
L62:
	popl	%ebx
	.loc 1 232 0
	leal	LC11-"L00000000011$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_puts
	.loc 1 233 0
	movl	$8, -12(%ebp)
	jmp	L56
L57:
	.loc 1 234 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movzbl	(%edx,%eax), %eax
	testb	%al, %al
	je	L58
	.loc 1 238 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movb	$0, (%edx,%eax)
	.loc 1 239 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	addl	$32, %eax
	movw	$0, 8(%eax)
	.loc 1 240 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	addl	$32, %eax
	movb	$0, 10(%eax)
	.loc 1 241 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	addl	$32, %eax
	movb	$0, 11(%eax)
	.loc 1 243 0
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	leal	-4(%eax), %edx
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	.loc 1 244 0
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %ecx
	movl	-12(%ebp), %eax
	imull	$52, %eax, %eax
	movl	%eax, %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, (%ecx)
	.loc 1 246 0
	leal	L_FreeTasks$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzwl	(%eax), %eax
	leal	1(%eax), %edx
	leal	L_FreeTasks$non_lazy_ptr-"L00000000011$pb"(%ebx), %eax
	movl	(%eax), %eax
	movw	%dx, (%eax)
L58:
	.loc 1 233 0
	incl	-12(%ebp)
L56:
	cmpl	$15, -12(%ebp)
	jle	L57
	.loc 1 249 0
	addl	$36, %esp
	popl	%ebx
	leave
	ret
LFE13:
.globl _create_task
_create_task:
LFB14:
	.loc 1 251 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI46:
	movl	%esp, %ebp
LCFI47:
	pushl	%esi
LCFI48:
	pushl	%ebx
LCFI49:
	subl	$32, %esp
LCFI50:
	call	L65
"L00000000012$pb":
L65:
	popl	%ebx
	movl	12(%ebp), %eax
	movl	16(%ebp), %edx
	movl	20(%ebp), %ecx
	movl	24(%ebp), %esi
	movw	%ax, -12(%ebp)
	movw	%dx, -16(%ebp)
	movb	%cl, -20(%ebp)
	movl	%esi, %eax
	movb	%al, -24(%ebp)
	.loc 1 261 0
	movswl	-12(%ebp),%edx
	movl	8(%ebp), %ecx
	leal	L_Tasks$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	addl	$20, %eax
	movl	%ecx, (%eax)
	.loc 1 262 0
	movswl	-12(%ebp),%edx
	leal	L_Tasks$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movb	$12, (%edx,%eax)
	.loc 1 263 0
	movswl	-12(%ebp),%edx
	leal	L_Tasks$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	leal	32(%eax), %edx
	movzwl	-16(%ebp), %eax
	movw	%ax, 8(%edx)
	.loc 1 264 0
	movswl	-12(%ebp),%edx
	leal	L_Tasks$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	leal	32(%eax), %edx
	movzbl	-20(%ebp), %eax
	movb	%al, 10(%edx)
	.loc 1 265 0
	movswl	-12(%ebp),%edx
	leal	L_Tasks$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	leal	32(%eax), %edx
	movzbl	-24(%ebp), %eax
	movb	%al, 11(%edx)
	.loc 1 267 0
	movswl	-12(%ebp),%edx
	leal	L_Tasks$non_lazy_ptr-"L00000000012$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movb	$0, 2(%edx,%eax)
	.loc 1 269 0
	movswl	-12(%ebp),%eax
	movl	%eax, (%esp)
	call	_RHCreateThread
	.loc 1 271 0
	addl	$32, %esp
	popl	%ebx
	popl	%esi
	leave
	ret
LFE14:
	.cstring
LC12:
	.ascii "!!!: NULL task %d\12\0"
	.text
_despatch_tasks:
LFB15:
	.loc 1 276 0
	nop
	nop
	nop
	nop
	nop
	nop
	pushl	%ebp
LCFI51:
	movl	%esp, %ebp
LCFI52:
	pushl	%ebx
LCFI53:
	subl	$36, %esp
LCFI54:
	call	L78
"L00000000013$pb":
L78:
	popl	%ebx
	.loc 1 283 0
	movl	$0, -12(%ebp)
	jmp	L67
L68:
	.loc 1 293 0
	movl	-12(%ebp), %eax
	movl	%eax, %edx
	leal	L_CurrentTask$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	movb	%dl, (%eax)
	.loc 1 294 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movzbl	(%edx,%eax), %eax
	cmpb	$3, %al
	jbe	L69
	.loc 1 295 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movzbl	(%edx,%eax), %eax
	cmpb	$4, %al
	jne	L71
	.loc 1 296 0
	movl	-12(%ebp), %eax
	leal	L_Tasks$non_lazy_ptr-"L00000000013$pb"(%ebx), %edx
	movl	(%edx), %edx
	imull	$52, %eax, %eax
	movb	$8, (%eax,%edx)
L71:
	.loc 1 299 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	leal	(%edx,%eax), %eax
	addl	$20, %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	L73
	.loc 1 300 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movb	$0, (%edx,%eax)
	.loc 1 301 0
	movl	-12(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	LC12-"L00000000013$pb"(%ebx), %eax
	movl	%eax, (%esp)
	call	_printf
	jmp	L69
L73:
	.loc 1 328 0
	movl	-12(%ebp), %eax
	imull	$52, %eax, %eax
	movl	%eax, %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, (%esp)
	call	_RHResume
	.loc 1 330 0
	movl	-12(%ebp), %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	imull	$52, %edx, %edx
	movzbl	(%edx,%eax), %eax
	testb	%al, %al
	jne	L69
	.loc 1 331 0
	leal	L_FreeTasks$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	movzwl	(%eax), %eax
	leal	1(%eax), %edx
	leal	L_FreeTasks$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	movw	%dx, (%eax)
	.loc 1 332 0
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %eax
	leal	-4(%eax), %edx
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	%edx, (%eax)
	.loc 1 333 0
	leal	L_NextFreeTask$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	movl	(%eax), %ecx
	movl	-12(%ebp), %eax
	imull	$52, %eax, %eax
	movl	%eax, %edx
	leal	L_Tasks$non_lazy_ptr-"L00000000013$pb"(%ebx), %eax
	movl	(%eax), %eax
	leal	(%edx,%eax), %eax
	movl	%eax, (%ecx)
L69:
	.loc 1 283 0
	incl	-12(%ebp)
L67:
	cmpl	$15, -12(%ebp)
	jle	L68
	.loc 1 351 0
	addl	$36, %esp
	popl	%ebx
	leave
	ret
LFE15:
.comm _Tasks,832,5
.comm _CurrentTask,1,0
.comm _EffectIsSetUp,1,0
.comm _NextFreeTask,4,2
.comm _FreeTasks,2,1
.comm _x820e,1,0
.comm _FreeTaskStack,32,5
	.section __DWARF,__debug_frame,regular,debug
Lframe0:
	.set L$set$0,LECIE0-LSCIE0
	.long L$set$0
LSCIE0:
	.long	0xffffffff
	.byte	0x1
	.ascii "\0"
	.byte	0x1
	.byte	0x7c
	.byte	0x8
	.byte	0xc
	.byte	0x4
	.byte	0x4
	.byte	0x88
	.byte	0x1
	.align 2
LECIE0:
LSFDE0:
	.set L$set$1,LEFDE0-LASFDE0
	.long L$set$1
LASFDE0:
	.set L$set$2,Lframe0-Lsection__debug_frame
	.long L$set$2
	.long	LFB3
	.set L$set$3,LFE3-LFB3
	.long L$set$3
	.byte	0x4
	.set L$set$4,LCFI0-LFB3
	.long L$set$4
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$5,LCFI1-LCFI0
	.long L$set$5
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$6,LCFI5-LCFI1
	.long L$set$6
	.byte	0x83
	.byte	0x5
	.byte	0x86
	.byte	0x4
	.byte	0x87
	.byte	0x3
	.align 2
LEFDE0:
LSFDE2:
	.set L$set$7,LEFDE2-LASFDE2
	.long L$set$7
LASFDE2:
	.set L$set$8,Lframe0-Lsection__debug_frame
	.long L$set$8
	.long	LFB4
	.set L$set$9,LFE4-LFB4
	.long L$set$9
	.byte	0x4
	.set L$set$10,LCFI6-LFB4
	.long L$set$10
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$11,LCFI7-LCFI6
	.long L$set$11
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$12,LCFI9-LCFI7
	.long L$set$12
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE2:
LSFDE4:
	.set L$set$13,LEFDE4-LASFDE4
	.long L$set$13
LASFDE4:
	.set L$set$14,Lframe0-Lsection__debug_frame
	.long L$set$14
	.long	LFB5
	.set L$set$15,LFE5-LFB5
	.long L$set$15
	.byte	0x4
	.set L$set$16,LCFI10-LFB5
	.long L$set$16
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$17,LCFI11-LCFI10
	.long L$set$17
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$18,LCFI13-LCFI11
	.long L$set$18
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE4:
LSFDE6:
	.set L$set$19,LEFDE6-LASFDE6
	.long L$set$19
LASFDE6:
	.set L$set$20,Lframe0-Lsection__debug_frame
	.long L$set$20
	.long	LFB6
	.set L$set$21,LFE6-LFB6
	.long L$set$21
	.byte	0x4
	.set L$set$22,LCFI14-LFB6
	.long L$set$22
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$23,LCFI15-LCFI14
	.long L$set$23
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$24,LCFI17-LCFI15
	.long L$set$24
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE6:
LSFDE8:
	.set L$set$25,LEFDE8-LASFDE8
	.long L$set$25
LASFDE8:
	.set L$set$26,Lframe0-Lsection__debug_frame
	.long L$set$26
	.long	LFB7
	.set L$set$27,LFE7-LFB7
	.long L$set$27
	.byte	0x4
	.set L$set$28,LCFI18-LFB7
	.long L$set$28
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$29,LCFI19-LCFI18
	.long L$set$29
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$30,LCFI21-LCFI19
	.long L$set$30
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE8:
LSFDE10:
	.set L$set$31,LEFDE10-LASFDE10
	.long L$set$31
LASFDE10:
	.set L$set$32,Lframe0-Lsection__debug_frame
	.long L$set$32
	.long	LFB8
	.set L$set$33,LFE8-LFB8
	.long L$set$33
	.byte	0x4
	.set L$set$34,LCFI22-LFB8
	.long L$set$34
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$35,LCFI23-LCFI22
	.long L$set$35
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$36,LCFI25-LCFI23
	.long L$set$36
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE10:
LSFDE12:
	.set L$set$37,LEFDE12-LASFDE12
	.long L$set$37
LASFDE12:
	.set L$set$38,Lframe0-Lsection__debug_frame
	.long L$set$38
	.long	LFB9
	.set L$set$39,LFE9-LFB9
	.long L$set$39
	.byte	0x4
	.set L$set$40,LCFI26-LFB9
	.long L$set$40
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$41,LCFI27-LCFI26
	.long L$set$41
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$42,LCFI29-LCFI27
	.long L$set$42
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE12:
LSFDE14:
	.set L$set$43,LEFDE14-LASFDE14
	.long L$set$43
LASFDE14:
	.set L$set$44,Lframe0-Lsection__debug_frame
	.long L$set$44
	.long	LFB10
	.set L$set$45,LFE10-LFB10
	.long L$set$45
	.byte	0x4
	.set L$set$46,LCFI30-LFB10
	.long L$set$46
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$47,LCFI31-LCFI30
	.long L$set$47
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$48,LCFI33-LCFI31
	.long L$set$48
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE14:
LSFDE16:
	.set L$set$49,LEFDE16-LASFDE16
	.long L$set$49
LASFDE16:
	.set L$set$50,Lframe0-Lsection__debug_frame
	.long L$set$50
	.long	LFB11
	.set L$set$51,LFE11-LFB11
	.long L$set$51
	.byte	0x4
	.set L$set$52,LCFI34-LFB11
	.long L$set$52
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$53,LCFI35-LCFI34
	.long L$set$53
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$54,LCFI37-LCFI35
	.long L$set$54
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE16:
LSFDE18:
	.set L$set$55,LEFDE18-LASFDE18
	.long L$set$55
LASFDE18:
	.set L$set$56,Lframe0-Lsection__debug_frame
	.long L$set$56
	.long	LFB12
	.set L$set$57,LFE12-LFB12
	.long L$set$57
	.byte	0x4
	.set L$set$58,LCFI38-LFB12
	.long L$set$58
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$59,LCFI39-LCFI38
	.long L$set$59
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$60,LCFI41-LCFI39
	.long L$set$60
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE18:
LSFDE20:
	.set L$set$61,LEFDE20-LASFDE20
	.long L$set$61
LASFDE20:
	.set L$set$62,Lframe0-Lsection__debug_frame
	.long L$set$62
	.long	LFB13
	.set L$set$63,LFE13-LFB13
	.long L$set$63
	.byte	0x4
	.set L$set$64,LCFI42-LFB13
	.long L$set$64
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$65,LCFI43-LCFI42
	.long L$set$65
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$66,LCFI45-LCFI43
	.long L$set$66
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE20:
LSFDE22:
	.set L$set$67,LEFDE22-LASFDE22
	.long L$set$67
LASFDE22:
	.set L$set$68,Lframe0-Lsection__debug_frame
	.long L$set$68
	.long	LFB14
	.set L$set$69,LFE14-LFB14
	.long L$set$69
	.byte	0x4
	.set L$set$70,LCFI46-LFB14
	.long L$set$70
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$71,LCFI47-LCFI46
	.long L$set$71
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$72,LCFI50-LCFI47
	.long L$set$72
	.byte	0x83
	.byte	0x4
	.byte	0x86
	.byte	0x3
	.align 2
LEFDE22:
LSFDE24:
	.set L$set$73,LEFDE24-LASFDE24
	.long L$set$73
LASFDE24:
	.set L$set$74,Lframe0-Lsection__debug_frame
	.long L$set$74
	.long	LFB15
	.set L$set$75,LFE15-LFB15
	.long L$set$75
	.byte	0x4
	.set L$set$76,LCFI51-LFB15
	.long L$set$76
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.set L$set$77,LCFI52-LCFI51
	.long L$set$77
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.set L$set$78,LCFI54-LCFI52
	.long L$set$78
	.byte	0x83
	.byte	0x3
	.align 2
LEFDE24:
	.text
Letext0:
	.section __DWARF,__debug_loc,regular,debug
Ldebug_loc0:
LLST0:
	.set L$set$79,LFB3-Ltext0
	.long L$set$79
	.set L$set$80,LCFI0-Ltext0
	.long L$set$80
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$81,LCFI0-Ltext0
	.long L$set$81
	.set L$set$82,LCFI1-Ltext0
	.long L$set$82
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$83,LCFI1-Ltext0
	.long L$set$83
	.set L$set$84,LFE3-Ltext0
	.long L$set$84
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST1:
	.set L$set$85,LFB4-Ltext0
	.long L$set$85
	.set L$set$86,LCFI6-Ltext0
	.long L$set$86
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$87,LCFI6-Ltext0
	.long L$set$87
	.set L$set$88,LCFI7-Ltext0
	.long L$set$88
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$89,LCFI7-Ltext0
	.long L$set$89
	.set L$set$90,LFE4-Ltext0
	.long L$set$90
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST2:
	.set L$set$91,LFB5-Ltext0
	.long L$set$91
	.set L$set$92,LCFI10-Ltext0
	.long L$set$92
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$93,LCFI10-Ltext0
	.long L$set$93
	.set L$set$94,LCFI11-Ltext0
	.long L$set$94
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$95,LCFI11-Ltext0
	.long L$set$95
	.set L$set$96,LFE5-Ltext0
	.long L$set$96
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST3:
	.set L$set$97,LFB6-Ltext0
	.long L$set$97
	.set L$set$98,LCFI14-Ltext0
	.long L$set$98
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$99,LCFI14-Ltext0
	.long L$set$99
	.set L$set$100,LCFI15-Ltext0
	.long L$set$100
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$101,LCFI15-Ltext0
	.long L$set$101
	.set L$set$102,LFE6-Ltext0
	.long L$set$102
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST4:
	.set L$set$103,LFB7-Ltext0
	.long L$set$103
	.set L$set$104,LCFI18-Ltext0
	.long L$set$104
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$105,LCFI18-Ltext0
	.long L$set$105
	.set L$set$106,LCFI19-Ltext0
	.long L$set$106
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$107,LCFI19-Ltext0
	.long L$set$107
	.set L$set$108,LFE7-Ltext0
	.long L$set$108
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST5:
	.set L$set$109,LFB8-Ltext0
	.long L$set$109
	.set L$set$110,LCFI22-Ltext0
	.long L$set$110
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$111,LCFI22-Ltext0
	.long L$set$111
	.set L$set$112,LCFI23-Ltext0
	.long L$set$112
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$113,LCFI23-Ltext0
	.long L$set$113
	.set L$set$114,LFE8-Ltext0
	.long L$set$114
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST6:
	.set L$set$115,LFB9-Ltext0
	.long L$set$115
	.set L$set$116,LCFI26-Ltext0
	.long L$set$116
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$117,LCFI26-Ltext0
	.long L$set$117
	.set L$set$118,LCFI27-Ltext0
	.long L$set$118
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$119,LCFI27-Ltext0
	.long L$set$119
	.set L$set$120,LFE9-Ltext0
	.long L$set$120
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST7:
	.set L$set$121,LFB10-Ltext0
	.long L$set$121
	.set L$set$122,LCFI30-Ltext0
	.long L$set$122
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$123,LCFI30-Ltext0
	.long L$set$123
	.set L$set$124,LCFI31-Ltext0
	.long L$set$124
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$125,LCFI31-Ltext0
	.long L$set$125
	.set L$set$126,LFE10-Ltext0
	.long L$set$126
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST8:
	.set L$set$127,LFB11-Ltext0
	.long L$set$127
	.set L$set$128,LCFI34-Ltext0
	.long L$set$128
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$129,LCFI34-Ltext0
	.long L$set$129
	.set L$set$130,LCFI35-Ltext0
	.long L$set$130
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$131,LCFI35-Ltext0
	.long L$set$131
	.set L$set$132,LFE11-Ltext0
	.long L$set$132
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST9:
	.set L$set$133,LFB12-Ltext0
	.long L$set$133
	.set L$set$134,LCFI38-Ltext0
	.long L$set$134
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$135,LCFI38-Ltext0
	.long L$set$135
	.set L$set$136,LCFI39-Ltext0
	.long L$set$136
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$137,LCFI39-Ltext0
	.long L$set$137
	.set L$set$138,LFE12-Ltext0
	.long L$set$138
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST10:
	.set L$set$139,LFB13-Ltext0
	.long L$set$139
	.set L$set$140,LCFI42-Ltext0
	.long L$set$140
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$141,LCFI42-Ltext0
	.long L$set$141
	.set L$set$142,LCFI43-Ltext0
	.long L$set$142
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$143,LCFI43-Ltext0
	.long L$set$143
	.set L$set$144,LFE13-Ltext0
	.long L$set$144
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST11:
	.set L$set$145,LFB14-Ltext0
	.long L$set$145
	.set L$set$146,LCFI46-Ltext0
	.long L$set$146
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$147,LCFI46-Ltext0
	.long L$set$147
	.set L$set$148,LCFI47-Ltext0
	.long L$set$148
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$149,LCFI47-Ltext0
	.long L$set$149
	.set L$set$150,LFE14-Ltext0
	.long L$set$150
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
LLST12:
	.set L$set$151,LFB15-Ltext0
	.long L$set$151
	.set L$set$152,LCFI51-Ltext0
	.long L$set$152
	.word	0x2
	.byte	0x74
	.byte	0x4
	.set L$set$153,LCFI51-Ltext0
	.long L$set$153
	.set L$set$154,LCFI52-Ltext0
	.long L$set$154
	.word	0x2
	.byte	0x74
	.byte	0x8
	.set L$set$155,LCFI52-Ltext0
	.long L$set$155
	.set L$set$156,LFE15-Ltext0
	.long L$set$156
	.word	0x2
	.byte	0x75
	.byte	0x8
	.long	0x0
	.long	0x0
	.file 2 "/Users/ben/Documents/dev/MT2/FistBlue/task.h"
	.section __DWARF,__debug_info,regular,debug
	.long	0x672
	.word	0x2
	.set L$set$157,Ldebug_abbrev0-Lsection__debug_abbrev
	.long L$set$157
	.byte	0x4
	.byte	0x1
	.ascii "GNU C 4.2.1 (Apple Inc. build 5664)\0"
	.byte	0x1
	.ascii "/Users/ben/Documents/dev/MT2/FistBlue/task.c\0"
	.long	Ltext0
	.long	Letext0
	.set L$set$158,Ldebug_line0-Lsection__debug_line
	.long L$set$158
	.byte	0x2
	.byte	0x1
	.byte	0x6
	.ascii "signed char\0"
	.byte	0x2
	.byte	0x1
	.byte	0x8
	.ascii "unsigned char\0"
	.byte	0x2
	.byte	0x2
	.byte	0x5
	.ascii "short int\0"
	.byte	0x2
	.byte	0x2
	.byte	0x7
	.ascii "short unsigned int\0"
	.byte	0x2
	.byte	0x4
	.byte	0x5
	.ascii "int\0"
	.byte	0x2
	.byte	0x4
	.byte	0x7
	.ascii "unsigned int\0"
	.byte	0x2
	.byte	0x8
	.byte	0x5
	.ascii "long long int\0"
	.byte	0x2
	.byte	0x8
	.byte	0x7
	.ascii "long long unsigned int\0"
	.byte	0x2
	.byte	0x4
	.byte	0x5
	.ascii "long int\0"
	.byte	0x3
	.byte	0x4
	.byte	0x7
	.byte	0x2
	.byte	0x1
	.byte	0x6
	.ascii "char\0"
	.byte	0x2
	.byte	0x4
	.byte	0x7
	.ascii "long unsigned int\0"
	.byte	0x4
	.byte	0x4
	.byte	0x5
	.byte	0x4
	.long	0x123
	.byte	0x6
	.long	0xfe
	.byte	0x5
	.byte	0x4
	.long	0x97
	.byte	0x5
	.byte	0x4
	.long	0x134
	.byte	0x6
	.long	0x97
	.byte	0x5
	.byte	0x4
	.long	0x13f
	.byte	0x6
	.long	0x79
	.byte	0x7
	.ascii "task_params\0"
	.byte	0x8
	.byte	0x2
	.byte	0x1a
	.long	0x1bc
	.byte	0x8
	.ascii "Param0\0"
	.byte	0x2
	.byte	0x1b
	.long	0x97
	.byte	0x2
	.byte	0x23
	.byte	0x0
	.byte	0x8
	.ascii "Param1\0"
	.byte	0x2
	.byte	0x1c
	.long	0x79
	.byte	0x2
	.byte	0x23
	.byte	0x2
	.byte	0x8
	.ascii "Param2\0"
	.byte	0x2
	.byte	0x1d
	.long	0x79
	.byte	0x2
	.byte	0x23
	.byte	0x3
	.byte	0x8
	.ascii "x0014\0"
	.byte	0x2
	.byte	0x1e
	.long	0x79
	.byte	0x2
	.byte	0x23
	.byte	0x4
	.byte	0x8
	.ascii "x0016\0"
	.byte	0x2
	.byte	0x1f
	.long	0x79
	.byte	0x2
	.byte	0x23
	.byte	0x5
	.byte	0x8
	.ascii "x001f\0"
	.byte	0x2
	.byte	0x20
	.long	0x79
	.byte	0x2
	.byte	0x23
	.byte	0x6
	.byte	0x0
	.byte	0x9
	.ascii "TaskParam\0"
	.byte	0x2
	.byte	0x22
	.long	0x144
	.byte	0xa
	.byte	0x34
	.byte	0x2
	.byte	0x25
	.long	0x2e5
	.byte	0x8
	.ascii "status\0"
	.byte	0x2
	.byte	0x26
	.long	0x79
	.byte	0x2
	.byte	0x23
	.byte	0x0
	.byte	0x8
	.ascii "timer\0"
	.byte	0x2
	.byte	0x27
	.long	0x79
	.byte	0x2
	.byte	0x23
	.byte	0x1
	.byte	0x8
	.ascii "mode0\0"
	.byte	0x2
	.byte	0x2a
	.long	0x79
	.byte	0x2
	.byte	0x23
	.byte	0x2
	.byte	0x8
	.ascii "mode1\0"
	.byte	0x2
	.byte	0x2b
	.long	0x79
	.byte	0x2
	.byte	0x23
	.byte	0x3
	.byte	0x8
	.ascii "gfx_p\0"
	.byte	0x2
	.byte	0x2c
	.long	0x128
	.byte	0x2
	.byte	0x23
	.byte	0x4
	.byte	0x8
	.ascii "counter\0"
	.byte	0x2
	.byte	0x2d
	.long	0x79
	.byte	0x2
	.byte	0x23
	.byte	0x8
	.byte	0x8
	.ascii "palette\0"
	.byte	0x2
	.byte	0x2e
	.long	0x97
	.byte	0x2
	.byte	0x23
	.byte	0xa
	.byte	0x8
	.ascii "X\0"
	.byte	0x2
	.byte	0x2f
	.long	0x8a
	.byte	0x2
	.byte	0x23
	.byte	0xc
	.byte	0x8
	.ascii "Y\0"
	.byte	0x2
	.byte	0x2f
	.long	0x8a
	.byte	0x2
	.byte	0x23
	.byte	0xe
	.byte	0x8
	.ascii "cp\0"
	.byte	0x2
	.byte	0x30
	.long	0xb4
	.byte	0x2
	.byte	0x23
	.byte	0x10
	.byte	0x8
	.ascii "code\0"
	.byte	0x2
	.byte	0x31
	.long	0x2ec
	.byte	0x2
	.byte	0x23
	.byte	0x14
	.byte	0x8
	.ascii "name\0"
	.byte	0x2
	.byte	0x32
	.long	0x11d
	.byte	0x2
	.byte	0x23
	.byte	0x18
	.byte	0x8
	.ascii "signal\0"
	.byte	0x2
	.byte	0x33
	.long	0x11d
	.byte	0x2
	.byte	0x23
	.byte	0x1c
	.byte	0x8
	.ascii "data\0"
	.byte	0x2
	.byte	0x35
	.long	0x139
	.byte	0x2
	.byte	0x23
	.byte	0x20
	.byte	0x8
	.ascii "data16\0"
	.byte	0x2
	.byte	0x36
	.long	0x12e
	.byte	0x2
	.byte	0x23
	.byte	0x24
	.byte	0x8
	.ascii "params\0"
	.byte	0x2
	.byte	0x42
	.long	0x1bc
	.byte	0x2
	.byte	0x23
	.byte	0x28
	.byte	0x8
	.ascii "RHThreadID\0"
	.byte	0x2
	.byte	0x45
	.long	0xfe
	.byte	0x2
	.byte	0x23
	.byte	0x30
	.byte	0x0
	.byte	0xb
	.long	0x2ec
	.byte	0xc
	.byte	0x0
	.byte	0x5
	.byte	0x4
	.long	0x2e5
	.byte	0x9
	.ascii "Task\0"
	.byte	0x2
	.byte	0x49
	.long	0x1cd
	.byte	0xd
	.byte	0x1
	.set L$set$159,LASF0-Lsection__debug_str
	.long L$set$159
	.byte	0x1
	.byte	0x3d
	.byte	0x1
	.long	LFB3
	.long	LFE3
	.set L$set$160,LLST0-Lsection__debug_loc
	.long L$set$160
	.long	0x324
	.byte	0xe
	.ascii "i\0"
	.byte	0x1
	.byte	0x3e
	.long	0xad
	.byte	0x2
	.byte	0x91
	.byte	0x5c
	.byte	0x0
	.byte	0xd
	.byte	0x1
	.set L$set$161,LASF1-Lsection__debug_str
	.long L$set$161
	.byte	0x1
	.byte	0x4c
	.byte	0x1
	.long	LFB4
	.long	LFE4
	.set L$set$162,LLST1-Lsection__debug_loc
	.long L$set$162
	.long	0x34a
	.byte	0xe
	.ascii "i\0"
	.byte	0x1
	.byte	0x4d
	.long	0xad
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x0
	.byte	0xf
	.byte	0x1
	.set L$set$163,LASF2-Lsection__debug_str
	.long L$set$163
	.byte	0x1
	.byte	0x55
	.long	LFB5
	.long	LFE5
	.set L$set$164,LLST2-Lsection__debug_loc
	.long L$set$164
	.long	0x372
	.byte	0xe
	.ascii "task\0"
	.byte	0x1
	.byte	0x56
	.long	0x372
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x0
	.byte	0x5
	.byte	0x4
	.long	0x2f2
	.byte	0xf
	.byte	0x1
	.set L$set$165,LASF3-Lsection__debug_str
	.long L$set$165
	.byte	0x1
	.byte	0x5d
	.long	LFB6
	.long	LFE6
	.set L$set$166,LLST3-Lsection__debug_loc
	.long L$set$166
	.long	0x3a0
	.byte	0xe
	.ascii "task\0"
	.byte	0x1
	.byte	0x5e
	.long	0x372
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x0
	.byte	0xd
	.byte	0x1
	.set L$set$167,LASF4-Lsection__debug_str
	.long L$set$167
	.byte	0x1
	.byte	0x69
	.byte	0x1
	.long	LFB7
	.long	LFE7
	.set L$set$168,LLST4-Lsection__debug_loc
	.long L$set$168
	.long	0x3f2
	.byte	0x10
	.ascii "code\0"
	.byte	0x1
	.byte	0x69
	.long	0x11b
	.byte	0x2
	.byte	0x91
	.byte	0x0
	.byte	0x10
	.ascii "p1\0"
	.byte	0x1
	.byte	0x69
	.long	0x97
	.byte	0x2
	.byte	0x91
	.byte	0x5c
	.byte	0x10
	.ascii "p2\0"
	.byte	0x1
	.byte	0x69
	.long	0x97
	.byte	0x2
	.byte	0x91
	.byte	0x58
	.byte	0xe
	.ascii "task\0"
	.byte	0x1
	.byte	0x6b
	.long	0x372
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x0
	.byte	0xd
	.byte	0x1
	.set L$set$169,LASF5-Lsection__debug_str
	.long L$set$169
	.byte	0x1
	.byte	0x83
	.byte	0x1
	.long	LFB8
	.long	LFE8
	.set L$set$170,LLST5-Lsection__debug_loc
	.long L$set$170
	.long	0x422
	.byte	0x11
	.long	LBB2
	.long	LBE2
	.byte	0xe
	.ascii "i\0"
	.byte	0x1
	.byte	0x84
	.long	0xad
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x0
	.byte	0x0
	.byte	0xd
	.byte	0x1
	.set L$set$171,LASF6-Lsection__debug_str
	.long L$set$171
	.byte	0x1
	.byte	0x98
	.byte	0x1
	.long	LFB9
	.long	LFE9
	.set L$set$172,LLST6-Lsection__debug_loc
	.long L$set$172
	.long	0x44f
	.byte	0x10
	.ascii "duration\0"
	.byte	0x1
	.byte	0x98
	.long	0xad
	.byte	0x2
	.byte	0x91
	.byte	0x0
	.byte	0x0
	.byte	0x12
	.byte	0x1
	.set L$set$173,LASF7-Lsection__debug_str
	.long L$set$173
	.byte	0x1
	.byte	0xc3
	.byte	0x1
	.long	LFB10
	.long	LFE10
	.set L$set$174,LLST7-Lsection__debug_loc
	.long L$set$174
	.byte	0x12
	.byte	0x1
	.set L$set$175,LASF8-Lsection__debug_str
	.long L$set$175
	.byte	0x1
	.byte	0xd0
	.byte	0x1
	.long	LFB11
	.long	LFE11
	.set L$set$176,LLST8-Lsection__debug_loc
	.long L$set$176
	.byte	0xd
	.byte	0x1
	.set L$set$177,LASF9-Lsection__debug_str
	.long L$set$177
	.byte	0x1
	.byte	0xdd
	.byte	0x1
	.long	LFB12
	.long	LFE12
	.set L$set$178,LLST9-Lsection__debug_loc
	.long L$set$178
	.long	0x4a0
	.byte	0x10
	.ascii "id\0"
	.byte	0x1
	.byte	0xdd
	.long	0x97
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x0
	.byte	0xd
	.byte	0x1
	.set L$set$179,LASF10-Lsection__debug_str
	.long L$set$179
	.byte	0x1
	.byte	0xe6
	.byte	0x1
	.long	LFB13
	.long	LFE13
	.set L$set$180,LLST10-Lsection__debug_loc
	.long L$set$180
	.long	0x4c6
	.byte	0xe
	.ascii "i\0"
	.byte	0x1
	.byte	0xe7
	.long	0xad
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x0
	.byte	0xd
	.byte	0x1
	.set L$set$181,LASF11-Lsection__debug_str
	.long L$set$181
	.byte	0x1
	.byte	0xfb
	.byte	0x1
	.long	LFB14
	.long	LFE14
	.set L$set$182,LLST11-Lsection__debug_loc
	.long L$set$182
	.long	0x532
	.byte	0x10
	.ascii "task\0"
	.byte	0x1
	.byte	0xfb
	.long	0x11b
	.byte	0x2
	.byte	0x91
	.byte	0x0
	.byte	0x10
	.ascii "taskid\0"
	.byte	0x1
	.byte	0xfb
	.long	0x8a
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x10
	.ascii "param\0"
	.byte	0x1
	.byte	0xfb
	.long	0x97
	.byte	0x2
	.byte	0x91
	.byte	0x68
	.byte	0x10
	.ascii "param1\0"
	.byte	0x1
	.byte	0xfb
	.long	0x79
	.byte	0x2
	.byte	0x91
	.byte	0x64
	.byte	0x10
	.ascii "param2\0"
	.byte	0x1
	.byte	0xfb
	.long	0x79
	.byte	0x2
	.byte	0x91
	.byte	0x60
	.byte	0x0
	.byte	0x13
	.set L$set$183,LASF12-Lsection__debug_str
	.long L$set$183
	.byte	0x1
	.word	0x114
	.byte	0x1
	.long	LFB15
	.long	LFE15
	.set L$set$184,LLST12-Lsection__debug_loc
	.long L$set$184
	.long	0x559
	.byte	0x14
	.ascii "i\0"
	.byte	0x1
	.word	0x115
	.long	0xad
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x0
	.byte	0x15
	.long	0xad
	.long	0x564
	.byte	0x16
	.byte	0x0
	.byte	0x17
	.ascii "__CFConstantStringClassReference\0"
	.long	0x559
	.byte	0x1
	.byte	0x1
	.byte	0x1
	.byte	0x18
	.ascii "gtimercount\0"
	.byte	0x1
	.byte	0x24
	.long	0xad
	.byte	0x1
	.byte	0x1
	.byte	0x15
	.long	0x2f2
	.long	0x5b2
	.byte	0x19
	.long	0xfb
	.byte	0xf
	.byte	0x0
	.byte	0x1a
	.ascii "Tasks\0"
	.byte	0x1
	.byte	0x26
	.long	0x5a2
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	_Tasks
	.byte	0x1a
	.ascii "CurrentTask\0"
	.byte	0x1
	.byte	0x27
	.long	0xfe
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	_CurrentTask
	.byte	0x1a
	.ascii "EffectIsSetUp\0"
	.byte	0x1
	.byte	0x28
	.long	0xfe
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	_EffectIsSetUp
	.byte	0x1a
	.ascii "NextFreeTask\0"
	.byte	0x1
	.byte	0x2a
	.long	0x617
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	_NextFreeTask
	.byte	0x5
	.byte	0x4
	.long	0x372
	.byte	0x1a
	.ascii "FreeTasks\0"
	.byte	0x1
	.byte	0x2b
	.long	0x97
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	_FreeTasks
	.byte	0x1a
	.ascii "x820e\0"
	.byte	0x1
	.byte	0x2c
	.long	0x79
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	_x820e
	.byte	0x15
	.long	0x372
	.long	0x659
	.byte	0x19
	.long	0xfb
	.byte	0x7
	.byte	0x0
	.byte	0x1a
	.ascii "FreeTaskStack\0"
	.byte	0x1
	.byte	0x31
	.long	0x649
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.long	_FreeTaskStack
	.byte	0x0
	.section __DWARF,__debug_abbrev,regular,debug
	.byte	0x1
	.byte	0x11
	.byte	0x1
	.byte	0x25
	.byte	0x8
	.byte	0x13
	.byte	0xb
	.byte	0x3
	.byte	0x8
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x10
	.byte	0x6
	.byte	0x0
	.byte	0x0
	.byte	0x2
	.byte	0x24
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x3e
	.byte	0xb
	.byte	0x3
	.byte	0x8
	.byte	0x0
	.byte	0x0
	.byte	0x3
	.byte	0x24
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x3e
	.byte	0xb
	.byte	0x0
	.byte	0x0
	.byte	0x4
	.byte	0xf
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x0
	.byte	0x0
	.byte	0x5
	.byte	0xf
	.byte	0x0
	.byte	0xb
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x6
	.byte	0x26
	.byte	0x0
	.byte	0x49
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x7
	.byte	0x13
	.byte	0x1
	.byte	0x3
	.byte	0x8
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x8
	.byte	0xd
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x38
	.byte	0xa
	.byte	0x0
	.byte	0x0
	.byte	0x9
	.byte	0x16
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0xa
	.byte	0x13
	.byte	0x1
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0xb
	.byte	0x15
	.byte	0x1
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0xc
	.byte	0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0xd
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0xc
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x27
	.byte	0xc
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x40
	.byte	0x6
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0xe
	.byte	0x34
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0xa
	.byte	0x0
	.byte	0x0
	.byte	0xf
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0xc
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x40
	.byte	0x6
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x10
	.byte	0x5
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0xa
	.byte	0x0
	.byte	0x0
	.byte	0x11
	.byte	0xb
	.byte	0x1
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.byte	0x12
	.byte	0x2e
	.byte	0x0
	.byte	0x3f
	.byte	0xc
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x27
	.byte	0xc
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x40
	.byte	0x6
	.byte	0x0
	.byte	0x0
	.byte	0x13
	.byte	0x2e
	.byte	0x1
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x27
	.byte	0xc
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x1
	.byte	0x40
	.byte	0x6
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x14
	.byte	0x34
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0xa
	.byte	0x0
	.byte	0x0
	.byte	0x15
	.byte	0x1
	.byte	0x1
	.byte	0x49
	.byte	0x13
	.byte	0x1
	.byte	0x13
	.byte	0x0
	.byte	0x0
	.byte	0x16
	.byte	0x21
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0x17
	.byte	0x34
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x49
	.byte	0x13
	.byte	0x3f
	.byte	0xc
	.byte	0x34
	.byte	0xc
	.byte	0x3c
	.byte	0xc
	.byte	0x0
	.byte	0x0
	.byte	0x18
	.byte	0x34
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x3f
	.byte	0xc
	.byte	0x3c
	.byte	0xc
	.byte	0x0
	.byte	0x0
	.byte	0x19
	.byte	0x21
	.byte	0x0
	.byte	0x49
	.byte	0x13
	.byte	0x2f
	.byte	0xb
	.byte	0x0
	.byte	0x0
	.byte	0x1a
	.byte	0x34
	.byte	0x0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x3f
	.byte	0xc
	.byte	0x2
	.byte	0xa
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section __DWARF,__debug_pubnames,regular,debug
	.long	0x127
	.word	0x2
	.set L$set$185,Ldebug_info0-Lsection__debug_info
	.long L$set$185
	.long	0x676
	.long	0x2fe
	.ascii "task_timer\0"
	.long	0x324
	.ascii "printtasktable\0"
	.long	0x34a
	.ascii "justdie\0"
	.long	0x378
	.ascii "diefree\0"
	.long	0x3a0
	.ascii "wrap_trap7\0"
	.long	0x3f2
	.ascii "print_task_table\0"
	.long	0x422
	.ascii "sf2sleep\0"
	.long	0x44f
	.ascii "exit_ready\0"
	.long	0x464
	.ascii "task_die\0"
	.long	0x479
	.ascii "task_kill\0"
	.long	0x4a0
	.ascii "die_top8\0"
	.long	0x4c6
	.ascii "create_task\0"
	.long	0x5b2
	.ascii "Tasks\0"
	.long	0x5c6
	.ascii "CurrentTask\0"
	.long	0x5e0
	.ascii "EffectIsSetUp\0"
	.long	0x5fc
	.ascii "NextFreeTask\0"
	.long	0x61d
	.ascii "FreeTasks\0"
	.long	0x635
	.ascii "x820e\0"
	.long	0x659
	.ascii "FreeTaskStack\0"
	.long	0x0
	.section __DWARF,__debug_pubtypes,regular,debug
	.long	0x35
	.word	0x2
	.set L$set$186,Ldebug_info0-Lsection__debug_info
	.long L$set$186
	.long	0x676
	.long	0x144
	.ascii "task_params\0"
	.long	0x1bc
	.ascii "TaskParam\0"
	.long	0x2f2
	.ascii "Task\0"
	.long	0x0
	.section __DWARF,__debug_aranges,regular,debug
	.long	0x1c
	.word	0x2
	.set L$set$187,Ldebug_info0-Lsection__debug_info
	.long L$set$187
	.byte	0x4
	.byte	0x0
	.word	0x0
	.word	0x0
	.long	Ltext0
	.set L$set$188,Letext0-Ltext0
	.long L$set$188
	.long	0x0
	.long	0x0
	.section __DWARF,__debug_str,regular,debug
LASF8:
	.ascii "task_die\0"
LASF9:
	.ascii "task_kill\0"
LASF3:
	.ascii "diefree\0"
LASF7:
	.ascii "exit_ready\0"
LASF0:
	.ascii "task_timer\0"
LASF4:
	.ascii "wrap_trap7\0"
LASF5:
	.ascii "print_task_table\0"
LASF1:
	.ascii "printtasktable\0"
LASF12:
	.ascii "despatch_tasks\0"
LASF10:
	.ascii "die_top8\0"
LASF2:
	.ascii "justdie\0"
LASF11:
	.ascii "create_task\0"
LASF6:
	.ascii "sf2sleep\0"
	.section __IMPORT,__pointers,non_lazy_symbol_pointers
L_gtimercount$non_lazy_ptr:
	.indirect_symbol _gtimercount
	.long	0
L_CurrentTask$non_lazy_ptr:
	.indirect_symbol _CurrentTask
	.long	0
L_FreeTasks$non_lazy_ptr:
	.indirect_symbol _FreeTasks
	.long	0
L_Tasks$non_lazy_ptr:
	.indirect_symbol _Tasks
	.long	0
L_NextFreeTask$non_lazy_ptr:
	.indirect_symbol _NextFreeTask
	.long	0
	.subsections_via_symbols
