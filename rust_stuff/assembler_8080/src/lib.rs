
#[derive(Debug, PartialEq)]
pub enum Register {
    A, // Accumulator
    B,
    C,
    D,
    E,
    H, // High 
    L, // Low
}

impl<'a> Register {
    fn parse(name: &str) -> Result<Register, String> {
        let register = match name.to_uppercase().as_str() {
            "A" => Register::A,
            "B" => Register::B,
            "C" => Register::C,
            "D" => Register::D,
            "E" => Register::E,
            "H" => Register::H,
            "L" => Register::L,
            _ => return Err(format!("Unknown Register: {}", name)),
        };

        Ok(register)
    }
}

#[derive(Debug, PartialEq)]
pub enum RegisterPair {
    B,      // B and C pair
    D,      // D and E pair
    H,      // High and low pair
    PSW,    // Accumulator and status flags
}

impl<'a> RegisterPair {
    fn parse(name: &str) -> Result<RegisterPair, String> {
        let pair = match name.to_uppercase().as_str() {
            "B" => RegisterPair::B,
            "D" => RegisterPair::D,
            "H" => RegisterPair::H,
            "PSW" => RegisterPair::PSW,
            _ => return Err(format!("Unknown Register Pair (rp): {}", name)),
        };

        Ok(pair)
    }
}

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
    JMP(u16),
    JNZ(u16),

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

impl Instruction<'_> {
    pub fn parse(line: &str) -> Result<Instruction<'_>, String> {
        // Split string into mnemonic and args
        let mut iter = line.split_whitespace();

        let mnemonic = iter
            .next()
            .expect("Instruction empty");

        let arguments: Vec<_> = iter.collect();

        let instruction = match mnemonic.to_uppercase().as_str() {
            "ADD" => {
                // ADD takes a register operand
                let register = Register::parse(arguments[0])?;
                Ok(Instruction::ADD(register))
            },
            _ => Err(format!("Unrecognised Mnemonic: {}", mnemonic))
        };

        instruction
    }
}

#[cfg(test)]
mod instruction_tests {
    use super::*;

    #[test]
    fn add_instruction() {
        let assembly = "ADD B";
        let instruction = Instruction::parse(assembly).unwrap();

        assert_eq!(
            instruction,
            Instruction::ADD(Register::B),
        );
    }

    #[test]
    #[should_panic(expected = "Instruction empty")]
    fn empty_input() {
        let assembly = "";
        let _ = Instruction::parse(assembly).expect("Failed to parse");
    }

    #[test]
    #[should_panic(expected = "Unknown Register: 9")]
    fn invalid_register() {
        let assembly = "ADD 9";
        let _ = Instruction::parse(assembly).unwrap();
    }
}

/*
pub enum Pseudo_Instruction {
    ORG(u16),
    EQU {label: &'a str, value: &'a str},
    END,
}

impl Pseudo_Instruction {
    fn parse(source: &'a str) -> Result<Pseudo_Instruction, String> {
    }
}

pub enum SectionData {
    Code(Vec<Instruction>),
    Data(Vec<u8>),
    Uninitialised(usize),
}

pub struct Section {
    pub name: &'a str,
    pub origin: u16,
    pub data: SectionData,
}

pub struct Program<'a> {
    pub equates: HashMap<&'a str, u16>,
    pub labels: HashMap<&'a str, u16>,
    pub blocks: Vec<Block>,
}
*/

