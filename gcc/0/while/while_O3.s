
a.out:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 2f 00 00 	mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__@Base>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	call   *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	ret

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 9a 2f 00 00    	push   0x2f9a(%rip)        # 3fc0 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	ff 25 9c 2f 00 00    	jmp    *0x2f9c(%rip)        # 3fc8 <_GLOBAL_OFFSET_TABLE_+0x10>
    102c:	0f 1f 40 00          	nopl   0x0(%rax)
    1030:	f3 0f 1e fa          	endbr64
    1034:	68 00 00 00 00       	push   $0x0
    1039:	e9 e2 ff ff ff       	jmp    1020 <_init+0x20>
    103e:	66 90                	xchg   %ax,%ax

Disassembly of section .plt.got:

0000000000001040 <__cxa_finalize@plt>:
    1040:	f3 0f 1e fa          	endbr64
    1044:	ff 25 ae 2f 00 00    	jmp    *0x2fae(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    104a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

Disassembly of section .plt.sec:

0000000000001050 <__printf_chk@plt>:
    1050:	f3 0f 1e fa          	endbr64
    1054:	ff 25 76 2f 00 00    	jmp    *0x2f76(%rip)        # 3fd0 <__printf_chk@GLIBC_2.3.4>
    105a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

Disassembly of section .text:

0000000000001060 <main>:
    1060:	f3 0f 1e fa          	endbr64
    1064:	48 83 ec 08          	sub    $0x8,%rsp
    1068:	ba 04 00 00 00       	mov    $0x4,%edx
    106d:	bf 02 00 00 00       	mov    $0x2,%edi
    1072:	31 c0                	xor    %eax,%eax
    1074:	48 8d 35 89 0f 00 00 	lea    0xf89(%rip),%rsi        # 2004 <_IO_stdin_used+0x4>
    107b:	e8 d0 ff ff ff       	call   1050 <__printf_chk@plt>
    1080:	ba 05 00 00 00       	mov    $0x5,%edx
    1085:	48 8d 35 83 0f 00 00 	lea    0xf83(%rip),%rsi        # 200f <_IO_stdin_used+0xf>
    108c:	31 c0                	xor    %eax,%eax
    108e:	bf 02 00 00 00       	mov    $0x2,%edi
    1093:	e8 b8 ff ff ff       	call   1050 <__printf_chk@plt>
    1098:	31 c0                	xor    %eax,%eax
    109a:	48 83 c4 08          	add    $0x8,%rsp
    109e:	c3                   	ret
    109f:	90                   	nop

00000000000010a0 <_start>:
    10a0:	f3 0f 1e fa          	endbr64
    10a4:	31 ed                	xor    %ebp,%ebp
    10a6:	49 89 d1             	mov    %rdx,%r9
    10a9:	5e                   	pop    %rsi
    10aa:	48 89 e2             	mov    %rsp,%rdx
    10ad:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    10b1:	50                   	push   %rax
    10b2:	54                   	push   %rsp
    10b3:	45 31 c0             	xor    %r8d,%r8d
    10b6:	31 c9                	xor    %ecx,%ecx
    10b8:	48 8d 3d a1 ff ff ff 	lea    -0x5f(%rip),%rdi        # 1060 <main>
    10bf:	ff 15 13 2f 00 00    	call   *0x2f13(%rip)        # 3fd8 <__libc_start_main@GLIBC_2.34>
    10c5:	f4                   	hlt
    10c6:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    10cd:	00 00 00 

00000000000010d0 <deregister_tm_clones>:
    10d0:	48 8d 3d 39 2f 00 00 	lea    0x2f39(%rip),%rdi        # 4010 <__TMC_END__>
    10d7:	48 8d 05 32 2f 00 00 	lea    0x2f32(%rip),%rax        # 4010 <__TMC_END__>
    10de:	48 39 f8             	cmp    %rdi,%rax
    10e1:	74 15                	je     10f8 <deregister_tm_clones+0x28>
    10e3:	48 8b 05 f6 2e 00 00 	mov    0x2ef6(%rip),%rax        # 3fe0 <_ITM_deregisterTMCloneTable@Base>
    10ea:	48 85 c0             	test   %rax,%rax
    10ed:	74 09                	je     10f8 <deregister_tm_clones+0x28>
    10ef:	ff e0                	jmp    *%rax
    10f1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    10f8:	c3                   	ret
    10f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001100 <register_tm_clones>:
    1100:	48 8d 3d 09 2f 00 00 	lea    0x2f09(%rip),%rdi        # 4010 <__TMC_END__>
    1107:	48 8d 35 02 2f 00 00 	lea    0x2f02(%rip),%rsi        # 4010 <__TMC_END__>
    110e:	48 29 fe             	sub    %rdi,%rsi
    1111:	48 89 f0             	mov    %rsi,%rax
    1114:	48 c1 ee 3f          	shr    $0x3f,%rsi
    1118:	48 c1 f8 03          	sar    $0x3,%rax
    111c:	48 01 c6             	add    %rax,%rsi
    111f:	48 d1 fe             	sar    $1,%rsi
    1122:	74 14                	je     1138 <register_tm_clones+0x38>
    1124:	48 8b 05 c5 2e 00 00 	mov    0x2ec5(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable@Base>
    112b:	48 85 c0             	test   %rax,%rax
    112e:	74 08                	je     1138 <register_tm_clones+0x38>
    1130:	ff e0                	jmp    *%rax
    1132:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    1138:	c3                   	ret
    1139:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001140 <__do_global_dtors_aux>:
    1140:	f3 0f 1e fa          	endbr64
    1144:	80 3d c5 2e 00 00 00 	cmpb   $0x0,0x2ec5(%rip)        # 4010 <__TMC_END__>
    114b:	75 2b                	jne    1178 <__do_global_dtors_aux+0x38>
    114d:	55                   	push   %rbp
    114e:	48 83 3d a2 2e 00 00 	cmpq   $0x0,0x2ea2(%rip)        # 3ff8 <__cxa_finalize@GLIBC_2.2.5>
    1155:	00 
    1156:	48 89 e5             	mov    %rsp,%rbp
    1159:	74 0c                	je     1167 <__do_global_dtors_aux+0x27>
    115b:	48 8b 3d a6 2e 00 00 	mov    0x2ea6(%rip),%rdi        # 4008 <__dso_handle>
    1162:	e8 d9 fe ff ff       	call   1040 <__cxa_finalize@plt>
    1167:	e8 64 ff ff ff       	call   10d0 <deregister_tm_clones>
    116c:	c6 05 9d 2e 00 00 01 	movb   $0x1,0x2e9d(%rip)        # 4010 <__TMC_END__>
    1173:	5d                   	pop    %rbp
    1174:	c3                   	ret
    1175:	0f 1f 00             	nopl   (%rax)
    1178:	c3                   	ret
    1179:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001180 <frame_dummy>:
    1180:	f3 0f 1e fa          	endbr64
    1184:	e9 77 ff ff ff       	jmp    1100 <register_tm_clones>
    1189:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001190 <While>:
    1190:	f3 0f 1e fa          	endbr64
    1194:	83 ff 05             	cmp    $0x5,%edi
    1197:	7e 77                	jle    1210 <While+0x80>
    1199:	8d 57 fa             	lea    -0x6(%rdi),%edx
    119c:	b9 cd cc cc cc       	mov    $0xcccccccd,%ecx
    11a1:	48 89 d0             	mov    %rdx,%rax
    11a4:	48 0f af d1          	imul   %rcx,%rdx
    11a8:	48 c1 ea 22          	shr    $0x22,%rdx
    11ac:	83 c2 01             	add    $0x1,%edx
    11af:	83 f8 0e             	cmp    $0xe,%eax
    11b2:	76 40                	jbe    11f4 <While+0x64>
    11b4:	66 0f 6e df          	movd   %edi,%xmm3
    11b8:	89 d1                	mov    %edx,%ecx
    11ba:	66 0f 6f 15 6e 0e 00 	movdqa 0xe6e(%rip),%xmm2        # 2030 <_IO_stdin_used+0x30>
    11c1:	00 
    11c2:	31 c0                	xor    %eax,%eax
    11c4:	66 0f 70 c3 00       	pshufd $0x0,%xmm3,%xmm0
    11c9:	66 0f fe 05 4f 0e 00 	paddd  0xe4f(%rip),%xmm0        # 2020 <_IO_stdin_used+0x20>
    11d0:	00 
    11d1:	c1 e9 02             	shr    $0x2,%ecx
    11d4:	0f 1f 40 00          	nopl   0x0(%rax)
    11d8:	83 c0 01             	add    $0x1,%eax
    11db:	66 0f 6f c8          	movdqa %xmm0,%xmm1
    11df:	66 0f fe c2          	paddd  %xmm2,%xmm0
    11e3:	39 c8                	cmp    %ecx,%eax
    11e5:	75 f1                	jne    11d8 <While+0x48>
    11e7:	f6 c2 03             	test   $0x3,%dl
    11ea:	74 2c                	je     1218 <While+0x88>
    11ec:	83 e2 fc             	and    $0xfffffffc,%edx
    11ef:	8d 04 92             	lea    (%rdx,%rdx,4),%eax
    11f2:	29 c7                	sub    %eax,%edi
    11f4:	8d 47 fb             	lea    -0x5(%rdi),%eax
    11f7:	83 f8 05             	cmp    $0x5,%eax
    11fa:	7e 16                	jle    1212 <While+0x82>
    11fc:	8d 47 f6             	lea    -0xa(%rdi),%eax
    11ff:	83 ef 0f             	sub    $0xf,%edi
    1202:	83 f8 05             	cmp    $0x5,%eax
    1205:	0f 4f c7             	cmovg  %edi,%eax
    1208:	c3                   	ret
    1209:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1210:	89 f8                	mov    %edi,%eax
    1212:	c3                   	ret
    1213:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    1218:	66 0f fe 0d 20 0e 00 	paddd  0xe20(%rip),%xmm1        # 2040 <_IO_stdin_used+0x40>
    121f:	00 
    1220:	66 0f 70 c9 ff       	pshufd $0xff,%xmm1,%xmm1
    1225:	66 0f 7e c8          	movd   %xmm1,%eax
    1229:	c3                   	ret
    122a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
# O3优化会直接把While和Goto展开，且不会调用函数
0000000000001230 <Goto>:
    1230:	f3 0f 1e fa          	endbr64
    1234:	83 ff 05             	cmp    $0x5,%edi
    1237:	7e 77                	jle    12b0 <Goto+0x80>
    1239:	8d 57 fa             	lea    -0x6(%rdi),%edx
    123c:	b9 cd cc cc cc       	mov    $0xcccccccd,%ecx
    1241:	48 89 d0             	mov    %rdx,%rax
    1244:	48 0f af d1          	imul   %rcx,%rdx
    1248:	48 c1 ea 22          	shr    $0x22,%rdx
    124c:	83 c2 01             	add    $0x1,%edx
    124f:	83 f8 0e             	cmp    $0xe,%eax
    1252:	76 40                	jbe    1294 <Goto+0x64>
    1254:	66 0f 6e df          	movd   %edi,%xmm3
    1258:	89 d1                	mov    %edx,%ecx
    125a:	66 0f 6f 15 ce 0d 00 	movdqa 0xdce(%rip),%xmm2        # 2030 <_IO_stdin_used+0x30>
    1261:	00 
    1262:	31 c0                	xor    %eax,%eax
    1264:	66 0f 70 c3 00       	pshufd $0x0,%xmm3,%xmm0
    1269:	66 0f fe 05 af 0d 00 	paddd  0xdaf(%rip),%xmm0        # 2020 <_IO_stdin_used+0x20>
    1270:	00 
    1271:	c1 e9 02             	shr    $0x2,%ecx
    1274:	0f 1f 40 00          	nopl   0x0(%rax)
    1278:	83 c0 01             	add    $0x1,%eax
    127b:	66 0f 6f c8          	movdqa %xmm0,%xmm1
    127f:	66 0f fe c2          	paddd  %xmm2,%xmm0
    1283:	39 c8                	cmp    %ecx,%eax
    1285:	75 f1                	jne    1278 <Goto+0x48>
    1287:	f6 c2 03             	test   $0x3,%dl
    128a:	74 2c                	je     12b8 <Goto+0x88>
    128c:	83 e2 fc             	and    $0xfffffffc,%edx
    128f:	8d 04 92             	lea    (%rdx,%rdx,4),%eax
    1292:	29 c7                	sub    %eax,%edi
    1294:	8d 47 fb             	lea    -0x5(%rdi),%eax
    1297:	83 f8 05             	cmp    $0x5,%eax
    129a:	7e 16                	jle    12b2 <Goto+0x82>
    129c:	8d 47 f6             	lea    -0xa(%rdi),%eax
    129f:	83 ef 0f             	sub    $0xf,%edi
    12a2:	83 f8 05             	cmp    $0x5,%eax
    12a5:	0f 4f c7             	cmovg  %edi,%eax
    12a8:	c3                   	ret
    12a9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    12b0:	89 f8                	mov    %edi,%eax
    12b2:	c3                   	ret
    12b3:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
    12b8:	66 0f fe 0d 80 0d 00 	paddd  0xd80(%rip),%xmm1        # 2040 <_IO_stdin_used+0x40>
    12bf:	00 
    12c0:	66 0f 70 c9 ff       	pshufd $0xff,%xmm1,%xmm1
    12c5:	66 0f 7e c8          	movd   %xmm1,%eax
    12c9:	c3                   	ret

Disassembly of section .fini:

00000000000012cc <_fini>:
    12cc:	f3 0f 1e fa          	endbr64
    12d0:	48 83 ec 08          	sub    $0x8,%rsp
    12d4:	48 83 c4 08          	add    $0x8,%rsp
    12d8:	c3                   	ret
