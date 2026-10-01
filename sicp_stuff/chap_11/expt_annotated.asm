
expt:     file format elf64-x86-64

This is the most basic function, there is no optimization

0000000000001159 <expt>:
	save return address
    1159:	55                   	push   %rbp
    115a:	48 89 e5             	mov    %rsp,%rbp

	copy function arguments to registers
    115d:	48 83 ec 10          	sub    $0x10,%rsp
    1161:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1164:	89 75 f8             	mov    %esi,-0x8(%rbp)

	check n == 0
    1167:	83 7d f8 00          	cmpl   $0x0,-0x8(%rbp)
    116b:	75 07                	jne    1174 <expt+0x1b>

	if n == 0 true jump to the return
    116d:	b8 01 00 00 00       	mov    $0x1,%eax
    1172:	eb 16                	jmp    118a <expt+0x31>

	otherwise setup arguments to call self again
    1174:	8b 45 f8             	mov    -0x8(%rbp),%eax
    1177:	8d 50 ff             	lea    -0x1(%rax),%edx
    117a:	8b 45 fc             	mov    -0x4(%rbp),%eax
    117d:	89 d6                	mov    %edx,%esi
    117f:	89 c7                	mov    %eax,%edi

	call function again consuming stack space
    1181:	e8 d3 ff ff ff       	call   1159 <expt>

	multiply comes after call meaning we cannot tail call optimize
    1186:	0f af 45 fc          	imul   -0x4(%rbp),%eax

	restore stack and return
    118a:	c9                   	leave
    118b:	c3                   	ret

000000000000118c <fast_expt_iter>:
	save return address
    118c:	55                   	push   %rbp
    118d:	48 89 e5             	mov    %rsp,%rbp

	copy function arguments to registers
    1190:	48 83 ec 10          	sub    $0x10,%rsp
    1194:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1197:	89 75 f8             	mov    %esi,-0x8(%rbp)
    119a:	89 55 f4             	mov    %edx,-0xc(%rbp)

	compare prod == 0
    119d:	83 7d f4 00          	cmpl   $0x0,-0xc(%rbp)
    11a1:	75 05                	jne    11a8 <fast_expt_iter+0x1c>

	else use a tail call optimized version, we'll just jump back to the start
	and keep executing
    11a3:	8b 45 f4             	mov    -0xc(%rbp),%eax
    11a6:	eb 1b                	jmp    11c3 <fast_expt_iter+0x37>

	compare prod == 0 true, now we 
    11a8:	8b 45 fc             	mov    -0x4(%rbp),%eax
    11ab:	0f af 45 f4          	imul   -0xc(%rbp),%eax
    11af:	89 c2                	mov    %eax,%edx
    11b1:	8b 45 f8             	mov    -0x8(%rbp),%eax
    11b4:	8d 48 ff             	lea    -0x1(%rax),%ecx
    11b7:	8b 45 fc             	mov    -0x4(%rbp),%eax
    11ba:	89 ce                	mov    %ecx,%esi
    11bc:	89 c7                	mov    %eax,%edi
    11be:	e8 c9 ff ff ff       	call   118c <fast_expt_iter>

	restore stack and return
    11c3:	c9                   	leave
    11c4:	c3                   	ret

00000000000011c5 <fast_expt>:
    11c5:	55                   	push   %rbp
    11c6:	48 89 e5             	mov    %rsp,%rbp
    11c9:	48 83 ec 20          	sub    $0x20,%rsp
    11cd:	89 7d ec             	mov    %edi,-0x14(%rbp)
    11d0:	89 75 e8             	mov    %esi,-0x18(%rbp)
    11d3:	83 7d e8 00          	cmpl   $0x0,-0x18(%rbp)
    11d7:	75 07                	jne    11e0 <fast_expt+0x1b>
    11d9:	b8 01 00 00 00       	mov    $0x1,%eax
    11de:	eb 43                	jmp    1223 <fast_expt+0x5e>
    11e0:	8b 45 e8             	mov    -0x18(%rbp),%eax
    11e3:	83 e0 01             	and    $0x1,%eax
    11e6:	85 c0                	test   %eax,%eax
    11e8:	75 25                	jne    120f <fast_expt+0x4a>
    11ea:	8b 45 e8             	mov    -0x18(%rbp),%eax
    11ed:	89 c2                	mov    %eax,%edx
    11ef:	c1 ea 1f             	shr    $0x1f,%edx
    11f2:	01 d0                	add    %edx,%eax
    11f4:	d1 f8                	sar    $1,%eax
    11f6:	89 c2                	mov    %eax,%edx
    11f8:	8b 45 ec             	mov    -0x14(%rbp),%eax
    11fb:	89 d6                	mov    %edx,%esi
    11fd:	89 c7                	mov    %eax,%edi
    11ff:	e8 c1 ff ff ff       	call   11c5 <fast_expt>
    1204:	89 45 fc             	mov    %eax,-0x4(%rbp)
    1207:	8b 45 fc             	mov    -0x4(%rbp),%eax
    120a:	0f af c0             	imul   %eax,%eax
    120d:	eb 14                	jmp    1223 <fast_expt+0x5e>
    120f:	8b 4d e8             	mov    -0x18(%rbp),%ecx
    1212:	8b 45 ec             	mov    -0x14(%rbp),%eax
    1215:	ba 01 00 00 00       	mov    $0x1,%edx
    121a:	89 ce                	mov    %ecx,%esi
    121c:	89 c7                	mov    %eax,%edi
    121e:	e8 69 ff ff ff       	call   118c <fast_expt_iter>
    1223:	c9                   	leave
    1224:	c3                   	ret

0000000000001225 <main>:
    1225:	55                   	push   %rbp
    1226:	48 89 e5             	mov    %rsp,%rbp
    1229:	48 83 ec 30          	sub    $0x30,%rsp
    122d:	89 7d dc             	mov    %edi,-0x24(%rbp)
    1230:	48 89 75 d0          	mov    %rsi,-0x30(%rbp)
    1234:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    123b:	00 00 
    123d:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    1241:	31 c0                	xor    %eax,%eax
    1243:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    1247:	48 83 c0 08          	add    $0x8,%rax
    124b:	48 8b 00             	mov    (%rax),%rax
    124e:	48 8d 55 ec          	lea    -0x14(%rbp),%rdx
    1252:	48 8d 0d ab 0d 00 00 	lea    0xdab(%rip),%rcx        # 2004 <_IO_stdin_used+0x4>
    1259:	48 89 ce             	mov    %rcx,%rsi
    125c:	48 89 c7             	mov    %rax,%rdi
    125f:	b8 00 00 00 00       	mov    $0x0,%eax
    1264:	e8 c7 fd ff ff       	call   1030 <__isoc23_sscanf@plt>
    1269:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    126d:	48 83 c0 10          	add    $0x10,%rax
    1271:	48 8b 00             	mov    (%rax),%rax
    1274:	48 8d 55 f0          	lea    -0x10(%rbp),%rdx
    1278:	48 8d 0d 85 0d 00 00 	lea    0xd85(%rip),%rcx        # 2004 <_IO_stdin_used+0x4>
    127f:	48 89 ce             	mov    %rcx,%rsi
    1282:	48 89 c7             	mov    %rax,%rdi
    1285:	b8 00 00 00 00       	mov    $0x0,%eax
    128a:	e8 a1 fd ff ff       	call   1030 <__isoc23_sscanf@plt>
    128f:	8b 55 f0             	mov    -0x10(%rbp),%edx
    1292:	8b 45 ec             	mov    -0x14(%rbp),%eax
    1295:	89 d6                	mov    %edx,%esi
    1297:	89 c7                	mov    %eax,%edi
    1299:	e8 27 ff ff ff       	call   11c5 <fast_expt>
    129e:	89 45 f4             	mov    %eax,-0xc(%rbp)
    12a1:	8b 45 f4             	mov    -0xc(%rbp),%eax
    12a4:	48 8d 15 5c 0d 00 00 	lea    0xd5c(%rip),%rdx        # 2007 <_IO_stdin_used+0x7>
    12ab:	89 c6                	mov    %eax,%esi
    12ad:	48 89 d7             	mov    %rdx,%rdi
    12b0:	b8 00 00 00 00       	mov    $0x0,%eax
    12b5:	e8 96 fd ff ff       	call   1050 <printf@plt>
    12ba:	b8 00 00 00 00       	mov    $0x0,%eax
    12bf:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    12c3:	64 48 2b 14 25 28 00 	sub    %fs:0x28,%rdx
    12ca:	00 00 
    12cc:	74 05                	je     12d3 <main+0xae>
    12ce:	e8 6d fd ff ff       	call   1040 <__stack_chk_fail@plt>
    12d3:	c9                   	leave
    12d4:	c3                   	ret
