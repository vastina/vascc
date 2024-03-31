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
