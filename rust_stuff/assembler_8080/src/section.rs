use crate::instruction::Instruction;
use std::collections::HashMap;

#[derive(Debug, PartialEq)]
pub enum SectionData<'a> {
    Code(Vec<Instruction<'a>>),
    Data(Vec<u8>),
    Uninitialised(u16),
}

use SectionData::*;

impl<'a> SectionData<'a> {
    pub fn size(&self) -> u16 {
        match self {
            Code(insts) => {
                insts
                    .iter()
                    .fold(0, |acc, inst: &Instruction| acc + inst.size())
            },
            Data(data) => { data.len().try_into().unwrap() },
            Uninitialised(size) => { *size }
        }
    }

    pub fn emit(
        &self,
        labels: HashMap<&'a str, u16>
    ) -> Vec<u8> {
        match self {
            Code(insts) => {
                insts
                    .iter()
                    .fold(Vec::new(), |acc: Vec<u8>, inst: &Instruction|
                            inst.emit(&labels)
                    )
            },
            Data(data) => { data.clone() },
            Uninitialised(size) => { vec![0; *size as usize] },
        }
    }
}

#[cfg(test)]
mod section_evaluate {
    use super::*;

    #[test]
    fn emit_code() {

        // No labels
        assert_eq!(
            Data(vec![0x01, 0x02]).emit(HashMap::new()),
            vec![0x01, 0x02],
        );

        // With labels

        assert_eq!(
            Data(vec![0x01, 0x02]).emit(HashMap::new()),
            vec![0x01, 0x02],
        );
    }

    #[test]
    fn emit_data() {
        assert_eq!(
            Data(vec![0x01, 0x02]).emit(HashMap::new()),
            vec![0x01, 0x02],
        );
    }

    #[test]
    fn emit_uninitialised() {
        assert_eq!(
            Uninitialised(0x0004).emit(HashMap::new()),
            vec![0x00, 0x00, 0x00, 0x00],
        );
    }
}

#[derive(Debug, PartialEq)]
pub struct Section<'a> {
    pub name: Option<&'a str>,
    pub origin: u16,
    pub data: SectionData<'a>,
}

impl<'a> Section<'a> {
    pub fn emit( &self, labels: HashMap<&'a str, u16>) -> Vec<u8> {
        self.data.emit(labels)
    }
}

