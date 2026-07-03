use crate::instruction::Instruction;
use std::collections::HashMap;

pub enum PseudoInstruction<'a> {
    ORG(u16),
    EQU {label: &'a str, value: &'a str},
    END,
}

impl<'a> PseudoInstruction<'_> {
    fn parse(_line: &'a str) -> Result<PseudoInstruction<'a>, String> {
        Ok(PseudoInstruction::END)
    }
}

pub enum SectionData<'a> {
    Code(Vec<Instruction<'a>>),
    Data(Vec<u8>),
    Uninitialised(usize),
}

pub struct Section<'a> {
    pub name: &'a str,
    pub origin: u16,
    pub data: SectionData<'a>,
}

pub struct Program<'a> {
    pub equates: HashMap<&'a str, u16>,
    pub labels: HashMap<&'a str, u16>,
    pub sections: Vec<Section<'a>>,
}

impl<'a> Program<'_> {
    /*
        First pass, build syntax tree, produce error messages, 
        like incorrect register types or bad values.
    */
    pub fn parse(&mut self, line: &'a str) -> Result<(), String> {
        // Remove comments
        let code = line.split(';').next().unwrap_or("").trim();
        if code.is_empty() {
            return Ok(());
        }

        // Check for a label such as "START: MVI A, 5"
        let instruction =  match code.split_once(':') {
            Some((l, r)) => {
                // Store label
                labels.push(l.trim());

                r.trim()
            },
            None => code,
        };

        // Attempt to parse as Instruction
        
        // Attempt to parse as Pseudo-Instruction
    }

    /*
        Second pass, convert syntax tree into bytes and replace symbols with
        proper 16-bit memory addresses. Check section collisions and final
        assembly of sections into binary file.
    */
    pub fn emit(&self) -> Result<Vec<u8>, String> {
    }
}

#[cfg(test)]
mod program_assemble {
    use super::*;

    #[test]
    fn add() {
        let pgm = Program {
            equates: HashMap::new(),
            labels: HashMap::new(),
            sections: vec![],
        };

        let code = "ORG 0100h ; Start of the program";

        Program::parse(code);
    }
}

