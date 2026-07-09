use crate::register::Register;
use crate::instruction::Instruction;

use std::collections::HashMap;

#[derive(Debug, PartialEq)]
pub enum PseudoInstruction<'a> {
    ORG(u16),
    EQU {label: String, value: String},
    END,
}

impl<'a> PseudoInstruction<'_> {
    fn parse(
        line: &'a str,
        equates: HashMap<&'a str, &'a str>,
    ) -> Result<PseudoInstruction<'a>, String> {
        let mut iter = line.split_whitespace();

        let arguments: Vec<_> = iter
            .into_iter()
            .collect::<Vec<_>>();

        let pseudo = if arugments.len() <= 2 {
            match arguments.to_uppercase().as_str() {
                "ORG" => {
                    let address = arguments[0].parse::<u16>()
                        .expect("failed to parse 16-bit address");

                    Ok(PseudoInstruction::ORG(address))
                },
                "END" => Ok(PseudoInstruction::END),
                _ => Err(format!("unrecognised pseudo-instruction: {}", line)),
            }
        } else if arguments.len() <= 3 {
            match arguments[1].to_uppercase().as_str() {
                "EQU" => {
                    PseudoInstruction::EQU(
                }
            }
        } else {
            Err(format!("unrecognised pseudo-instruction: {}", line)),
        }

        pseudo
    }
}

#[cfg(test)]
mod program_assemble {
    use super::*;

    #[test]
    fn parse_org() {
        let line = "ORG     0100h";
        assert_eq!(PseudoInstruction::parse(line), );
    }
}

#[derive(Debug, PartialEq)]
pub enum SectionData<'a> {
    Code(Vec<Instruction<'a>>),
    Data(Vec<u8>),
    Uninitialised(usize),
}

#[derive(Debug, PartialEq)]
pub struct Section<'a> {
    pub name: Option<String>,
    pub origin: u16,
    pub data: SectionData<'a>,
}

#[derive(Debug, PartialEq)]
pub struct Program<'a> {
    pub equates: HashMap<String, u16>,
    pub labels: HashMap<String, String>,
    pub sections: Vec<Section<'a>>,
}

impl<'a> Program<'a> {
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
        let (label, instruction) =  match code.split_once(':') {
            Some((l, i)) => {
                // Store label
                (l.trim(), i.trim())
            },
            None => ("", code),
        };

        if !label.is_empty() {
            // create a new section under this label
            // calculate label relative address
        };

        if !instruction.is_empty() {
            // 
        }

        Ok(())
    }

    /*
        Second pass, convert syntax tree into bytes and replace symbols with
        proper 16-bit memory addresses. Check section collisions and final
        assembly of sections into binary file.
    */
    pub fn emit(&self) -> Result<Vec<u8>, String> {
        Ok(vec![0xBA, 0xAD, 0xF0, 0x0D])
    }
}

#[cfg(test)]
mod program_assemble {
    use super::*;

    #[test]
    fn create_section() {
        let mut pgm = Program {
            equates: HashMap::new(),
            labels: HashMap::new(),
            sections: vec![],
        };

        let code = "MVI     B, 3";
        let _ = Program::parse(&mut pgm, code);

        assert!(pgm.sections.len() > 0);
        assert_eq!(
            pgm.sections[0].data,
            SectionData::Code(vec![Instruction::MVI(Register::B, 3)])
        );
    }
}

