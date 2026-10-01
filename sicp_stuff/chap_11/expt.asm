
expt:     file format elf64-x86-64

0000000000001200 <expt>:
    1200:	b8 01 00 00 00       	mov    $0x1,%eax
    1205:	85 f6                	test   %esi,%esi
    1207:	74 22                	je     122b <expt+0x2b>
    1209:	40 f6 c6 01          	test   $0x1,%sil
    120d:	74 11                	je     1220 <expt+0x20>
    120f:	89 f8                	mov    %edi,%eax
    1211:	83 ee 01             	sub    $0x1,%esi
    1214:	74 16                	je     122c <expt+0x2c>
    1216:	66 2e 0f 1f 84 00 00 	cs nopw 0x0(%rax,%rax,1)
    121d:	00 00 00 
    1220:	0f af c7             	imul   %edi,%eax
    1223:	0f af c7             	imul   %edi,%eax
    1226:	83 ee 02             	sub    $0x2,%esi
    1229:	75 f5                	jne    1220 <expt+0x20>
    122b:	c3                   	ret
    122c:	c3                   	ret
    122d:	0f 1f 00             	nopl   (%rax)

0000000000001230 <fast_expt_iter>:
    1230:	31 c0                	xor    %eax,%eax
    1232:	c3                   	ret

0000000000001240 <fast_expt>:
    1240:	b8 01 00 00 00       	mov    $0x1,%eax
    1245:	85 f6                	test   %esi,%esi
    1247:	74 12                	je     125b <fast_expt+0x1b>

    1249:	89 f0                	mov    %esi,%eax
    124b:	c1 e8 1f             	shr    $0x1f,%eax
    124e:	01 f0                	add    %esi,%eax
    1250:	d1 f8                	sar    $1,%eax
    1252:	09 f0                	or     %esi,%eax
    1254:	f7 d0                	not    %eax
    1256:	83 e0 01             	and    $0x1,%eax
    1259:	75 05                	jne    1260 <fast_expt+0x20>
    125b:	c3                   	ret

    125c:	0f 1f 40 00          	nopl   0x0(%rax)

    1260:	48 83 ec 08          	sub    $0x8,%rsp
    1264:	8d 46 03             	lea    0x3(%rsi),%eax
    1267:	85 f6                	test   %esi,%esi
    1269:	0f 48 f0             	cmovs  %eax,%esi
    126c:	c1 fe 02             	sar    $0x2,%esi

    126f:	e8 cc ff ff ff       	call   1240 <fast_expt>

    1274:	48 83 c4 08          	add    $0x8,%rsp
    1278:	0f af c0             	imul   %eax,%eax
    127b:	0f af c0             	imul   %eax,%eax
    127e:	c3                   	ret

