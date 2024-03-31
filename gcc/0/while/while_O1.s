
0000000000001149 <While>:
    1149:	f3 0f 1e fa          	endbr64
    114d:	89 f8                	mov    %edi,%eax

    114f:	83 ff 05             	cmp    $0x5,%edi
    1152:	7e 08                	jle    115c <While+0x13>
    1154:	83 e8 05             	sub    $0x5,%eax
    1157:	83 f8 05             	cmp    $0x5,%eax
    115a:	7f f8                	jg     1154 <While+0xb>

    115c:	c3                   	ret
# 开了基本的优化后指令数一样了
000000000000115d <Goto>:
    115d:	f3 0f 1e fa          	endbr64
    1161:	89 f8                	mov    %edi,%eax

    1163:	83 ff 05             	cmp    $0x5,%edi
    1166:	7e 08                	jle    1170 <Goto+0x13>
    1168:	83 e8 05             	sub    $0x5,%eax
    116b:	83 f8 05             	cmp    $0x5,%eax
    116e:	7f f8                	jg     1168 <Goto+0xb>

    1170:	c3                   	ret