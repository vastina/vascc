0000000000001149 <While>:
    1149:	f3 0f 1e fa          	endbr64
    114d:	55                   	push   %rbp
    114e:	48 89 e5             	mov    %rsp,%rbp

    1151:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1154:	eb 04                	jmp    115a <While+0x11>
    1156:	83 6d fc 05          	subl   $0x5,-0x4(%rbp)
    115a:	83 7d fc 05          	cmpl   $0x5,-0x4(%rbp)
    115e:	7f f6                	jg     1156 <While+0xd>
    1160:	8b 45 fc             	mov    -0x4(%rbp),%eax

    1163:	5d                   	pop    %rbp
    1164:	c3                   	ret

0000000000001165 <Goto>:
    1165:	f3 0f 1e fa          	endbr64
    1169:	55                   	push   %rbp
    116a:	48 89 e5             	mov    %rsp,%rbp

    116d:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1170:	83 7d fc 05          	cmpl   $0x5,-0x4(%rbp)
    1174:	7f 05                	jg     117b <Goto+0x16>
    1176:	8b 45 fc             	mov    -0x4(%rbp),%eax
    1179:	eb 07                	jmp    1182 <Goto+0x1d>
    117b:	90                   	nop
    117c:	83 6d fc 05          	subl   $0x5,-0x4(%rbp)
    1180:	eb ee                	jmp    1170 <Goto+0xb>

    1182:	5d                   	pop    %rbp
    1183:	c3                   	ret
