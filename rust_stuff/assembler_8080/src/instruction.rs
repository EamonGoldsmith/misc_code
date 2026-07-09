use crate::register::{ Register, RegisterPair };
use std::collections::HashMap;

/*
    Every "Code" Instrucion is represented here, that is, every instruction
    which can eventually be assembled into a byte(s). An instruction will
    always be created with its appropriate operands.
*/
#[derive(Debug, PartialEq)]
pub enum Instruction<'a> {
    // Data statements
    // ...

    // Carry bit instructions
    // ...

    // Single register instructions
    ORA(Register),

    // NOP instruction
    NOP,

    // Data transfer instructions
    MOV(Register, Register),

    // Register or memory to accumulator instructions
    ADD(Register),

    // Rotate accumulator instructions
    // ...
    
    // Register pair instructions
    DCX(RegisterPair),

    // Immediate instructions
    LXI(RegisterPair, u16),
    MVI(Register, u8),

    // Direct addressing instructions
    STA(u16),

    // Jump instructions
    JMP(&'a str),
    JNZ(&'a str),

    // Call subroutine instructions
    CALL(&'a str),

    // Return from subroutine instructions
    RET,

    // RST instruction
    RST,

    // Input/output instructions
    OUT(u8),

    // Halt instruction
    HLT,
}

impl<'a> Instruction<'_> {
    /*
        Parse expects a slice 'line' like "ADD A B" or "ORA (1 << 3)"
        and will produce the Instruction corresponding to the line.
        Will return an Err result and message if it is unable to parse.
    */
    pub fn parse(line: &str) -> Result<Instruction<'_>, String> {
        // Split string into mnemonic and args
        let mut iter = line.split_whitespace();

        let mnemonic = iter
            .next()
            .expect("instruction empty");

        let arguments: Vec<_> = iter
            .into_iter()
            .map(|tok| tok.trim_matches(','))
            .collect::<Vec<_>>();

        let instruction = match mnemonic.to_uppercase().as_str() {
            "ORA" => {
                let register = Register::parse(arguments[0])?;
                Instruction::ORA(register)
            },
            "NOP" => Instruction::NOP,
            "MOV" => {
                let dst = Register::parse(arguments[0])?;
                let src = Register::parse(arguments[1])?;
                Instruction::MOV(dst, src)
            },
            "ADD" => {
                let register = Register::parse(arguments[0])?;
                Instruction::ADD(register)
            },
            "DCX" => {
                let pair = RegisterPair::parse(arguments[0])?;
                Instruction::DCX(pair)
            },
            "LXI" => {
                let pair = RegisterPair::parse(arguments[0])?;
                let immediate = arguments[1].parse::<u16>()
                    .expect("failed to parse 16-bit immediate");
                Instruction::LXI(pair, immediate)
            },
            "MVI" => {
                let register = Register::parse(arguments[0])?;
                let immediate = arguments[1].parse::<u8>()
                    .expect("failed to parse 8-bit immediate");
                Instruction::MVI(register, immediate)
            },
            "STA" => {
                let immediate = arguments[0].parse::<u16>()
                    .expect("failed to parse 16-bit immediate");
                Instruction::STA(immediate)
            },
            "JMP" => {
                Instruction::JMP(arguments[0])
            },
            "JNZ" => {
                Instruction::JNZ(arguments[0])
            },
            "CALL" => {
                Instruction::CALL(arguments[0])
            },
            "RET" => Instruction::RET,
            "RST" => Instruction::RST,
            "OUT" => {
                let device = arguments[1].parse::<u8>()
                    .expect("failed to parse 8-bit immediate");
                Instruction::OUT(device)
            },
            "HLT" => Instruction::HLT,

            _ => return Err(format!("unrecognised mnemonic: {}", mnemonic)),
        };

        Ok(instruction)
    }
    /*
        Evaluate takes an Instruction and produces the corresponding bytes,
        will also evaluate any expressions in operands in this step.
        Returns Err and message if Expression fails to resolve.
    */
    fn emit(&self, labels: HashMap<&'a str, u16>) -> Result<Vec<u8>, String> {
        let hex = match self {
            Instruction::ORA(reg) => vec![0x83u8 | reg.emit()],
            Instruction::NOP => vec![0x00u8],
            Instruction::MOV(dst, src) => vec![
                0x00u8 | (dst.emit() << 3) | src.emit()
            ],
            Instruction::ADD(reg) => vec![0x80u8 | reg.emit()],
            Instruction::DCX(rp) => vec![0x00u8 | (rp.emit() << 4) | 0x0Bu8],
            Instruction::LXI(rp, valx) => vec![
                0x01 | (rp.emit() << 4),
                (valx & 0x00FFu16).try_into().unwrap(),
                ((valx & 0xFF00u16) >> 8).try_into().unwrap()
            ],
            Instruction::MVI(_reg, _val) => vec![0x00],
            Instruction::STA(_valx) => vec![0x00],
            Instruction::JMP(_sym) => vec![0x00],
            Instruction::JNZ(_sym) => vec![0x00],
            Instruction::CALL(_sym) => vec![0x00],
            Instruction::RET => vec![0x00],
            Instruction::RST => vec![0x00],
            Instruction::OUT(_dev) => vec![0x00],
            Instruction::HLT => vec![0x00],

            //_ => return Err(format!("unhandled instruction: {:?}", self)),
        };

        Ok(hex)
    }

    // return the size in bytes of the instruction
    pub fn size(&self) -> u16 {
        match self {
            Instruction::ORA(_) => 1,
            Instruction::NOP => 1,
            Instruction::MOV(_, _) => 1,
            Instruction::ADD(_) => 1,
            Instruction::DCX(_) => 1,
            Instruction::LXI(_, _) => 3,
            Instruction::MVI(_, _) => 2,
            Instruction::STA(_) => 3,
            Instruction::JMP(_) => 3,
            Instruction::JNZ(_) => 3,
            Instruction::CALL(_) => 3,
            Instruction::RET => 1,
            Instruction::RST => 1,
            Instruction::OUT(_) => 1,
            Instruction::HLT => 1,
        }
    }
}

#[cfg(test)]
mod instruction_parse {
    use super::*;

    #[test]
    fn add() {
        let instruction = Instruction::parse("ADD B").unwrap();
        assert_eq!(instruction, Instruction::ADD(Register::B));
    }

    #[test]
    fn lxi() {
        let instruction = Instruction::parse("LXI B, 32").unwrap();
        assert_eq!(instruction, Instruction::LXI(RegisterPair::B, 32u16));
    }

    #[test]
    fn lowercase() {
        let instruction = Instruction::parse("add c").unwrap();
        assert_eq!( instruction, Instruction::ADD(Register::C));
    }

    #[test]
    fn mixed_case() {
        let instruction = Instruction::parse("MvI b, 32").unwrap();
        assert_eq!(instruction, Instruction::MVI(Register::B, 32u8));
    }

    #[test]
    fn nop() {
        assert_eq!(Instruction::parse("NOP").unwrap(), Instruction::NOP);
    }

    #[test]
    #[should_panic(expected = "instruction empty")]
    fn empty_input() {
        let _ = Instruction::parse("").unwrap();
    }

    #[test]
    #[should_panic(expected = "unrecognised register: 9")]
    fn invalid_register() {
        let _ = Instruction::parse("ADD 9").unwrap();
    }

    #[test]
    #[should_panic(expected = "unrecognised mnemonic: ABOBA")]
    fn invalid_mnemonic() {
        let _ = Instruction::parse("ABOBA PSW").unwrap();
    }
}

#[cfg(test)]
mod instruction_evaluate {
    use super::*;
    
    #[test]
    fn add() {
        // Test doesn't use any symbols so pass empty shmap
        let symbols = HashMap::new();

        let hex: Vec<u8> = Instruction::ADD(Register::B)
            .emit(symbols)
            .unwrap();

        assert_eq!(vec![0x80], hex);
    }
}
