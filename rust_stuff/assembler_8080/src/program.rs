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
    pub blocks: Vec<Section<'a>>,
}
