use crate::instruction::Instruction;
use std::collections::HashMap;

#[derive(Debug, PartialEq)]
pub enum SectionData<'a> {
    Code(Vec<Instruction<'a>>),
    Data(Vec<u8>),
    Uninitialised(u16),
}

impl<'a> SectionData<'a> {
    pub fn size(&self) -> u16 {
        match self {
            SectionData::Code(instructions) => {
                instructions
                    .iter()
                    .fold(0, |acc, inst: &Instruction| acc + inst.size())
            },
            SectionData::Data(data) => {
                data.len().try_into().unwrap()
            },
            SectionData::Uninitialised(size) => {
                *size
            }
        }
    }

    pub fn emit(&self, labels: HashMap<&'a str, u16>) -> Result<Vec<u8>, String> {
        match self {
            SectionData::Code(instructions) => {
                let bytes = instructions
                    .iter()
                    .map(|inst| { inst.emit(labels) })
                    .collect();

                Ok(bytes)
            },
            SectionData::Data(data) => { Ok(*data) },
            SectionData::Uninitialised(_) => { Ok(vec![0; 10]) },
        }
    }
}

#[derive(Debug, PartialEq)]
pub struct Section<'a> {
    pub name: Option<&'a str>,
    pub origin: u16,
    pub data: SectionData<'a>,
}

impl<'a> Section<'a> {
    pub fn emit(
        &self,
        labels: HashMap<&'a str, u16>
    ) -> Result<Vec<u8>, String> {
        self.data.emit(labels)
    }
}

