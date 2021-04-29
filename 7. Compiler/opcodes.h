#pragma once
// ALL USEFUL MACHINE COMMANDS

#define NOP 0x90

// PUSH <REG>
#define PUSH_EAX 0x50
#define PUSH_ECX 0x51
#define PUSH_EDX 0x52
#define PUSH_EBX 0x53
#define PUSH_EBP 0x55
#define PUSH_ESI 0x56
#define PUSH_EDI 0x57

#define PUSH 0x68 

// POP <REG>

#define POP_EAX 0x58
#define POP_ECX 0x59
#define POP_EDX 0x5a
#define POP_EBX 0x5b
#define POP_EBP 0x5d
#define POP_ESI 0x5e
#define POP_EDI 0x5f

// MOV EAX, <REG>

#define MOV_EAX_EAX 0x8bc0
#define MOV_EAX_ECX 0x8bc1
#define MOV_EAX_EDX 0x8bc2
#define MOV_EAX_EBX 0x8bc3
#define MOV_EAX_ESP 0x8bc4
#define MOV_EAX_EBP 0x8bc5
#define MOV_EAX_ESI 0x8bc6
#define MOV_EAX_EDI 0x8bc7

// MOV ECX, <REG>

#define MOV_ECX_EAX 0x8bc8
#define MOV_ECX_ECX 0x8bc9
#define MOV_ECX_EDX 0x8bca
#define MOV_ECX_EBX 0x8bcb
#define MOV_ECX_ESP 0x8bcc
#define MOV_ECX_EBP 0x8bcd
#define MOV_ECX_ESI 0x8bce
#define MOV_ECX_EDI 0x8bcf

// MOV EDX, <REG>

#define MOV_EDX_EAX 0x8bd0
#define MOV_EDX_ECX 0x8bd1
#define MOV_EDX_EDX 0x8bd2
#define MOV_EDX_EBX 0x8bd3
#define MOV_EDX_ESP 0x8bd4
#define MOV_EDX_EBP 0x8bd5
#define MOV_EDX_ESI 0x8bd6
#define MOV_EDX_EDI 0x8bd7

// MOV EBX, <REG>

#define MOV_EBX_EAX 0x8bd8
#define MOV_EBX_ECX 0x8bd9
#define MOV_EBX_EDX 0x8bda
#define MOV_EBX_EBX 0x8bdb
#define MOV_EBX_ESP 0x8bdc
#define MOV_EBX_EBP 0x8bdd
#define MOV_EBX_ESI 0x8bde
#define MOV_EBX_EDI 0x8bdf

// MOV ESP, <REG>

#define MOV_ESP_EAX 0x8be0
#define MOV_ESP_ECX 0x8be1
#define MOV_ESP_EDX 0x8be2
#define MOV_ESP_EBX 0x8be3
#define MOV_ESP_ESP 0x8be4
#define MOV_ESP_EBP 0x8be5
#define MOV_ESP_ESI 0x8be6
#define MOV_ESP_EDI 0x8be7

// MOV EBP, <REG>

#define MOV_EBP_EAX 0x8be8
#define MOV_EBP_ECX 0x8be9
#define MOV_EBP_EDX 0x8bea
#define MOV_EBP_EBX 0x8beb
#define MOV_EBP_ESP 0x8bec
#define MOV_EBP_EBP 0x8bed
#define MOV_EBP_ESI 0x8bee
#define MOV_EBP_EDI 0x8bef

// MOV ESI, <REG>

#define MOV_ESI_EAX 0x8bf0
#define MOV_ESI_ECX 0x8bf1
#define MOV_ESI_EDX 0x8bf2
#define MOV_ESI_EBX 0x8bf3
#define MOV_ESI_ESP 0x8bf4
#define MOV_ESI_EBP 0x8bf5
#define MOV_ESI_ESI 0x8bf6
#define MOV_ESI_EDI 0x8bf7

// MOV EDI, <REG>

#define MOV_EDI_EAX 0x8bf8
#define MOV_EDI_ECX 0x8bf9
#define MOV_EDI_EDX 0x8bfa
#define MOV_EDI_EBX 0x8bfb
#define MOV_EDI_ESP 0x8bfc
#define MOV_EDI_EBP 0x8bfd
#define MOV_EDI_ESI 0x8bfe
#define MOV_EDI_EDI 0x8bff

// MOV NUM TO REG

#define MOV_EAX 0xb8
#define MOV_ECX 0xb9
#define MOV_EDX 0xba
#define MOV_EBX 0xbb
#define MOV_ESP 0xbc
#define MOV_EBP 0xbd
#define MOV_ESI 0xbe
#define MOV_EDI 0xbf

// ADD <REG>, EAX

#define ADD_EAX_EAX 0x01c0
#define ADD_ECX_EAX 0x01c1
#define ADD_EDX_EAX 0x01c2
#define ADD_EBX_EAX 0x01c3
#define ADD_ESP_EAX 0x01c4
#define ADD_EBP_EAX 0x01c5
#define ADD_ESI_EAX 0x01c6
#define ADD_EDI_EAX 0x01c7

// ADD <REG>, ECX

#define ADD_EAX_ECX 0x01c8
#define ADD_ECX_ECX 0x01c9
#define ADD_EDX_ECX 0x01ca
#define ADD_EBX_ECX 0x01cb
#define ADD_ESP_ECX 0x01cc
#define ADD_EBP_ECX 0x01cd
#define ADD_ESI_ECX 0x01ce
#define ADD_EDI_ECX 0x01cf

// ADD <REG>, EDX

#define ADD_EAX_EDX 0x01d0
#define ADD_ECX_EDX 0x01d1
#define ADD_EDX_EDX 0x01d2
#define ADD_EBX_EDX 0x01d3
#define ADD_ESP_EDX 0x01d4
#define ADD_EBP_EDX 0x01d5
#define ADD_ESI_EDX 0x01d6
#define ADD_EDI_EDX 0x01d7

// ADD <REG>, EBX

#define ADD_EAX_EBX 0x01d8
#define ADD_ECX_EBX 0x01d9
#define ADD_EDX_EBX 0x01da
#define ADD_EBX_EBX 0x01db
#define ADD_ESP_EBX 0x01dc
#define ADD_EBP_EBX 0x01dd
#define ADD_ESI_EBX 0x01de
#define ADD_EDI_EBX 0x01df

// ADD <REG>, ESP

#define ADD_EAX_ESP 0x01e0
#define ADD_ECX_ESP 0x01e1
#define ADD_EDX_ESP 0x01e2
#define ADD_EBX_ESP 0x01e3
#define ADD_ESP_ESP 0x01e4
#define ADD_EBP_ESP 0x01e5
#define ADD_ESI_ESP 0x01e6
#define ADD_EDI_ESP 0x01e7

// ADD <REG>, EBP

#define ADD_EAX_EBP 0x01e8
#define ADD_ECX_EBP 0x01e9
#define ADD_EDX_EBP 0x01ea
#define ADD_EBX_EBP 0x01eb
#define ADD_ESP_EBP 0x01ec
#define ADD_EBP_EBP 0x01ed
#define ADD_ESI_EBP 0x01ee
#define ADD_EDI_EBP 0x01ef

// ADD <REG>, ESI

#define ADD_EAX_ESI 0x01f0
#define ADD_ECX_ESI 0x01f1
#define ADD_EDX_ESI 0x01f2
#define ADD_EBX_ESI 0x01f3
#define ADD_ESP_ESI 0x01f4
#define ADD_EBP_ESI 0x01f5
#define ADD_ESI_ESI 0x01f6
#define ADD_EDI_ESI 0x01f7

// ADD <REG>, EDI

#define ADD_EAX_EDI 0x01f8
#define ADD_ECX_EDI 0x01f9
#define ADD_EDX_EDI 0x01fa
#define ADD_EBX_EDI 0x01fb
#define ADD_ESP_EDI 0x01fc
#define ADD_EBP_EDI 0x01fd
#define ADD_ESI_EDI 0x01fe
#define ADD_EDI_EDI 0x01ff

// ADD NUM TO REG

#define ADD_EAX 0x81c0
#define ADD_ECX 0x81c1
#define ADD_EDX 0x81c2
#define ADD_EBX 0x81c3
#define ADD_ESP 0x81c4
#define ADD_EBP 0x81c5
#define ADD_ESI 0x81c6
#define ADD_EDI 0x81c7

// SUB EAX, <REG>

#define SUB_EAX_EAX 0x2bc0
#define SUB_EAX_ECX 0x2bc1
#define SUB_EAX_EDX 0x2bc2
#define SUB_EAX_EBX 0x2bc3
#define SUB_EAX_ESP 0x2bc4
#define SUB_EAX_EBP 0x2bc5
#define SUB_EAX_ESI 0x2bc6
#define SUB_EAX_EDI 0x2bc7

// SUB ECX, <REG>

#define SUB_ECX_EAX 0x2bc8
#define SUB_ECX_ECX 0x2bc9
#define SUB_ECX_EDX 0x2bca
#define SUB_ECX_EBX 0x2bcb
#define SUB_ECX_ESP 0x2bcc
#define SUB_ECX_EBP 0x2bcd
#define SUB_ECX_ESI 0x2bce
#define SUB_ECX_EDI 0x2bcf

// SUB EDX, <REG>

#define SUB_EDX_EAX 0x2bd0
#define SUB_EDX_ECX 0x2bd1
#define SUB_EDX_EDX 0x2bd2
#define SUB_EDX_EBX 0x2bd3
#define SUB_EDX_ESP 0x2bd4
#define SUB_EDX_EBP 0x2bd5
#define SUB_EDX_ESI 0x2bd6
#define SUB_EDX_EDI 0x2bd7

// SUB EBX, <REG>

#define SUB_EBX_EAX 0x2bd8
#define SUB_EBX_ECX 0x2bd9
#define SUB_EBX_EDX 0x2bda
#define SUB_EBX_EBX 0x2bdb
#define SUB_EBX_ESP 0x2bdc
#define SUB_EBX_EBP 0x2bdd
#define SUB_EBX_ESI 0x2bde
#define SUB_EBX_EDI 0x2bdf

// SUB ESP, <REG>

#define SUB_ESP_EAX 0x2be0
#define SUB_ESP_ECX 0x2be1
#define SUB_ESP_EDX 0x2be2
#define SUB_ESP_EBX 0x2be3
#define SUB_ESP_ESP 0x2be4
#define SUB_ESP_EBP 0x2be5
#define SUB_ESP_ESI 0x2be6
#define SUB_ESP_EDI 0x2be7

// SUB EBP, <REG>

#define SUB_EBP_EAX 0x2be8
#define SUB_EBP_ECX 0x2be9
#define SUB_EBP_EDX 0x2bea
#define SUB_EBP_EBX 0x2beb
#define SUB_EBP_ESP 0x2bec
#define SUB_EBP_EBP 0x2bed
#define SUB_EBP_ESI 0x2bee
#define SUB_EBP_EDI 0x2bef

// SUB ESI, <REG>

#define SUB_ESI_EAX 0x2bf0
#define SUB_ESI_ECX 0x2bf1
#define SUB_ESI_EDX 0x2bf2
#define SUB_ESI_EBX 0x2bf3
#define SUB_ESI_ESP 0x2bf4
#define SUB_ESI_EBP 0x2bf5
#define SUB_ESI_ESI 0x2bf6
#define SUB_ESI_EDI 0x2bf7

// SUB EDI, <REG>

#define SUB_EDI_EAX 0x2bf8
#define SUB_EDI_ECX 0x2bf9
#define SUB_EDI_EDX 0x2bfa
#define SUB_EDI_EBX 0x2bfb
#define SUB_EDI_ESP 0x2bfc
#define SUB_EDI_EBP 0x2bfd
#define SUB_EDI_ESI 0x2bfe
#define SUB_EDI_EDI 0x2bff

// SUB NUM FROM REG

#define SUB_EAX 0x81e8
#define SUB_ECX 0x81e9
#define SUB_EDX 0x81ea
#define SUB_EBX 0x81eb
#define SUB_ESP 0x81ec
#define SUB_EBP 0x81ed
#define SUB_ESI 0x81ee
#define SUB_EDI 0x81ef

// CMP <REG>, EAX

#define CMP_EAX_EAX 0x39c0
#define CMP_ECX_EAX 0x39c1
#define CMP_EDX_EAX 0x39c2
#define CMP_EBX_EAX 0x39c3
#define CMP_ESP_EAX 0x39c4
#define CMP_EBP_EAX 0x39c5
#define CMP_ESI_EAX 0x39c6
#define CMP_EDI_EAX 0x39c7

// CMP <REG>, ECX

#define CMP_EAX_ECX 0x39c8
#define CMP_ECX_ECX 0x39c9
#define CMP_EDX_ECX 0x39ca
#define CMP_EBX_ECX 0x39cb
#define CMP_ESP_ECX 0x39cc
#define CMP_EBP_ECX 0x39cd
#define CMP_ESI_ECX 0x39ce
#define CMP_EDI_ECX 0x39cf

// CMP <REG>, EDX

#define CMP_EAX_EDX 0x39d0
#define CMP_ECX_EDX 0x39d1
#define CMP_EDX_EDX 0x39d2
#define CMP_EBX_EDX 0x39d3
#define CMP_ESP_EDX 0x39d4
#define CMP_EBP_EDX 0x39d5
#define CMP_ESI_EDX 0x39d6
#define CMP_EDI_EDX 0x39d7

// CMP <REG>, EBX

#define CMP_EAX_EBX 0x39d8
#define CMP_ECX_EBX 0x39d9
#define CMP_EDX_EBX 0x39da
#define CMP_EBX_EBX 0x39db
#define CMP_ESP_EBX 0x39dc
#define CMP_EBP_EBX 0x39dd
#define CMP_ESI_EBX 0x39de
#define CMP_EDI_EBX 0x39df

// CMP <REG>, ESP

#define CMP_EAX_ESP 0x39e0
#define CMP_ECX_ESP 0x39e1
#define CMP_EDX_ESP 0x39e2
#define CMP_EBX_ESP 0x39e3
#define CMP_ESP_ESP 0x39e4
#define CMP_EBP_ESP 0x39e5
#define CMP_ESI_ESP 0x39e6
#define CMP_EDI_ESP 0x39e7

// CMP <REG>, EBP

#define CMP_EAX_EBP 0x39e8
#define CMP_ECX_EBP 0x39e9
#define CMP_EDX_EBP 0x39ea
#define CMP_EBX_EBP 0x39eb
#define CMP_ESP_EBP 0x39ec
#define CMP_EBP_EBP 0x39ed
#define CMP_ESI_EBP 0x39ee
#define CMP_EDI_EBP 0x39ef

// CMP <REG>, ESI

#define CMP_EAX_ESI 0x39f0
#define CMP_ECX_ESI 0x39f1
#define CMP_EDX_ESI 0x39f2
#define CMP_EBX_ESI 0x39f3
#define CMP_ESP_ESI 0x39f4
#define CMP_EBP_ESI 0x39f5
#define CMP_ESI_ESI 0x39f6
#define CMP_EDI_ESI 0x39f7

// CMP <REG>, EDI

#define CMP_EAX_EDI 0x39f8
#define CMP_ECX_EDI 0x39f9
#define CMP_EDX_EDI 0x39fa
#define CMP_EBX_EDI 0x39fb
#define CMP_ESP_EDI 0x39fc
#define CMP_EBP_EDI 0x39fd
#define CMP_ESI_EDI 0x39fe
#define CMP_EDI_EDI 0x39ff

// CMP NUM WITH REG

#define CMP_EAX 0x81f8
#define CMP_ECX 0x81f9
#define CMP_EDX 0x81fa
#define CMP_EBX 0x81fb
#define CMP_ESP 0x81fc
#define CMP_EBP 0x81fd
#define CMP_ESI 0x81fe
#define CMP_EDI 0x81ff

// MUL <REG>

#define MUL_EAX 0xf7e0
#define MUL_ECX 0xf7e1
#define MUL_EDX 0xf7e2
#define MUL_EBX 0xf7e3
#define MUL_ESP 0xf7e4
#define MUL_EBP 0xf7e5
#define MUL_ESI 0xf7e6
#define MUL_EDI 0xf7e7

// DIV <REG>

#define DIV_EAX 0xf7f0
#define DIV_ECX 0xf7f1
#define DIV_EDX 0xf7f2
#define DIV_EBX 0xf7f3
#define DIV_ESP 0xf7f4
#define DIV_EBP 0xf7f5
#define DIV_ESI 0xf7f6
#define DIV_EDI 0xf7f7

// XOR EAX, <REG>

#define XOR_EAX_EAX 0x33c0
#define XOR_EAX_ECX 0x33c1
#define XOR_EAX_EDX 0x33c2
#define XOR_EAX_EBX 0x33c3
#define XOR_EAX_ESP 0x33c4
#define XOR_EAX_EBP 0x33c5
#define XOR_EAX_ESI 0x33c6
#define XOR_EAX_EDI 0x33c7

// XOR ECX, <REG>

#define XOR_ECX_EAX 0x33c8
#define XOR_ECX_ECX 0x33c9
#define XOR_ECX_EDX 0x33ca
#define XOR_ECX_EBX 0x33cb
#define XOR_ECX_ESP 0x33cc
#define XOR_ECX_EBP 0x33cd
#define XOR_ECX_ESI 0x33ce
#define XOR_ECX_EDI 0x33cf

// XOR EDX, <REG>

#define XOR_EDX_EAX 0x33d0
#define XOR_EDX_ECX 0x33d1
#define XOR_EDX_EDX 0x33d2
#define XOR_EDX_EBX 0x33d3
#define XOR_EDX_ESP 0x33d4
#define XOR_EDX_EBP 0x33d5
#define XOR_EDX_ESI 0x33d6
#define XOR_EDX_EDI 0x33d7

// XOR EBX, <REG>

#define XOR_EBX_EAX 0x33d8
#define XOR_EBX_ECX 0x33d9
#define XOR_EBX_EDX 0x33da
#define XOR_EBX_EBX 0x33db
#define XOR_EBX_ESP 0x33dc
#define XOR_EBX_EBP 0x33dd
#define XOR_EBX_ESI 0x33de
#define XOR_EBX_EDI 0x33df

// XOR ESP, <REG>

#define XOR_ESP_EAX 0x33e0
#define XOR_ESP_ECX 0x33e1
#define XOR_ESP_EDX 0x33e2
#define XOR_ESP_EBX 0x33e3
#define XOR_ESP_ESP 0x33e4
#define XOR_ESP_EBP 0x33e5
#define XOR_ESP_ESI 0x33e6
#define XOR_ESP_EDI 0x33e7

// XOR EBP, <REG>

#define XOR_EBP_EAX 0x33e8
#define XOR_EBP_ECX 0x33e9
#define XOR_EBP_EDX 0x33ea
#define XOR_EBP_EBX 0x33eb
#define XOR_EBP_ESP 0x33ec
#define XOR_EBP_EBP 0x33ed
#define XOR_EBP_ESI 0x33ee
#define XOR_EBP_EDI 0x33ef

// XOR ESI, <REG>

#define XOR_ESI_EAX 0x33f0
#define XOR_ESI_ECX 0x33f1
#define XOR_ESI_EDX 0x33f2
#define XOR_ESI_EBX 0x33f3
#define XOR_ESI_ESP 0x33f4
#define XOR_ESI_EBP 0x33f5
#define XOR_ESI_ESI 0x33f6
#define XOR_ESI_EDI 0x33f7

// XOR EDI, <REG>

#define XOR_EDI_EAX 0x33f8
#define XOR_EDI_ECX 0x33f9
#define XOR_EDI_EDX 0x33fa
#define XOR_EDI_EBX 0x33fb
#define XOR_EDI_ESP 0x33fc
#define XOR_EDI_EBP 0x33fd
#define XOR_EDI_ESI 0x33fe
#define XOR_EDI_EDI 0x33ff

// XOR NUM WITH REG

#define XOR_EAX 0x81f0
#define XOR_ECX 0x81f1
#define XOR_EDX 0x81f2
#define XOR_EBX 0x81f3
#define XOR_ESP 0x81f4
#define XOR_EBP 0x81f5
#define XOR_ESI 0x81f6
#define XOR_EDI 0x81f7

// JMP RELATIVE

#define JMP_REL 0xe9
#define JA_REL  0x0f87
#define JAE_REL 0x0f83
#define JB_REL  0x0f82
#define JBE_REL 0x0f86
#define JE_REL  0x0f84
#define JNE_REL 0x0f85

// CALL & RET

#define CALL_REL 0xe8
#define RET      0xc3
