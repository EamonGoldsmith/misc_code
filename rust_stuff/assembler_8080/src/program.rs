use crate:: {
    register::Register,
    instruction::Instruction,
    pseudo_instruction::PseudoInstruction,
    section::{ Section, SectionData },
};
use std::collections::HashMap;

#[derive(Debug, PartialEq)]
pub struct Program<'a> {
    pub equates: HashMap<&'a str, &'a str>,
    pub labels: HashMap<&'a str, u16>,
    pub sections: Vec<Section<'a>>,
}

impl<'a> Program<'a> {
    /*
        Create an empty program, with a code section starting at 0x00
    */
    pub fn new() -> Program<'a> {
        let first_section = Section {
            name: Some("root"),
            origin: 0u16,
            data: SectionData::Code(vec![]),
        };

        Program {
            equates: HashMap::new(),
            labels: HashMap::new(),
            sections: vec![first_section],
        }
    }

    /*
        First pass, build syntax tree, produce error messages, 
        like incorrect register types or bad values.
    */
    pub fn parse(&mut self, source: &'a str) -> Result<(), String> {
        for line in source.split('\n') {
            self.parse_line(line)?;
        }

        Ok(())
    }

    fn parse_line(&mut self, line: &'a str) -> Result<(), String> {
        // Remove comments
        let clean_line = line.split(';').next().unwrap_or("").trim();
        if clean_line.is_empty() {
            return Ok(());
        }

        // Try parsing as pseudo instruction
        if let Ok(pseudo) = PseudoInstruction::parse(clean_line) {
            match pseudo {
                PseudoInstruction::ORG(addr) => {
                    println!("org {}", addr)
                },
                PseudoInstruction::EQU {label: l, value: v} => {
                    println!("label {}, value {}", l, v)
                },
                PseudoInstruction::END => {
                    println!("found end?")
                },
            }

            return Ok(());
        }

        // Check for a label in the current line
        let (label, code) =  match clean_line.split_once(':') {
            Some((l, i)) => {
                (l.trim(), i.trim())
            },
            None => ("", clean_line),
        };

        // If there is a label parse it
        if !label.is_empty() {
            /*
                Determine the address of the label, we can't know its address in
                the whole program just in the current section.
                
                Future jumping instructions that use this label can look it up
                in the self.labels hashmap, to retrieve the jump address and
                section.
            */
            let address = self.sections.last()
                .map(|s| s.origin + s.data.size())
                .unwrap_or(0x0000u16);

            self.labels.insert(label, address);
        };

        // If there is code parse it
        if !code.is_empty() {
            // Pass on the error if instruction fails to parse
            let ins = Instruction::parse(code)?;
            self.push_instruction(ins)?;
        }

        Ok(())
    }

    fn push_instruction(
        &mut self,
        ins: Instruction<'a>
    ) -> Result<(), String> {

        // Ensure the current section is a code section
        let incompatible = match self.sections.last_mut() {
            None => true,
            Some(section) if !matches!(section.data, SectionData::Code(_)) => true,
            _ => false,
        };

        if incompatible {
            // Determine next origin address
            let next_origin = self.sections.last()
                .map(|s| s.origin + s.data.size())
                .unwrap_or(0x0000);

            // Create a code section for the next instruction
            self.sections.push(Section {
                name: None, // Anonymous section
                origin: next_origin,
                data: SectionData::Code(Vec::new()),
            });
        }

        // Add the instruction to the current code section
        if let Some(
            Section {data: SectionData::Code(insts), .. }
        ) = self.sections.last_mut() {
            insts.push(ins);
        }

        Ok(())
    }

    /*
        Second pass, convert syntax tree into bytes and replace symbols with
        proper 16-bit memory addresses. Check section collisions and final
        assembly of sections into binary file.
    */
    pub fn emit(&self) -> Vec<u8> {
        vec![0x00, 0x00]
    }
}

#[cfg(test)]
mod program_assemble {
    use super::*;

    #[test]
    fn parse_create_section() {
        let mut pgm = Program::new();
        let code = "  MVI     B, 3";
        let _ = Program::parse(&mut pgm, code);

        // Check code section is created with proper contents 
        assert!(pgm.sections.len() > 0);
        assert_eq!(
            pgm.sections[0].data,
            SectionData::Code(vec![Instruction::MVI(Register::B, 3)])
        );
    }

    #[test]
    fn parse_simple_label() {
        let mut pgm = Program::new();
        let source = "
              NOP
            LOOP: 
              add b, 1
              jmp LOOP
        ";

        let _ = Program::parse(&mut pgm, source);
        assert!(pgm.sections.len() > 0);

        // Check section contains correct instructions
        assert_eq!(
            pgm.sections[0].data,
            SectionData::Code(vec![
                Instruction::NOP,
                Instruction::ADD(Register::B),
                Instruction::JMP("LOOP")
            ])
        );

        // Check address of LOOP label matches expected byte offset
        assert_eq!(*pgm.labels.get("LOOP").unwrap(), 0x0001);
    }

    #[test]
    fn parse_multiple_label() {
        let mut pgm = Program::new();

        // Yes yes, I know it doesn't make sense :D
        let source = "
              NOP
            Wait:
              ADD A, 1 
              RST
            Loop: 
              ADD B, 1
              JMP Loop
        ";

        let _ = Program::parse(&mut pgm, source);

        assert_eq!(*pgm.labels.get("Wait").unwrap(), 0x0001);
        assert_eq!(*pgm.labels.get("Loop").unwrap(), 0x0003);
    }

    #[test]
    fn parse_add() {
        let mut pgm = Program::new();

        // Assemble the example blink program, expect no panic
        let source = "
; Intel 8080 Assembly Language Example 1
; This program adds two numbers (5 and 3) and stores the result.

        ORG     100         ; Start of the program (standard CP/M transient program area)

START:  MVI     A, 5        ; Move immediate value 5 into register A (Accumulator)
        MVI     B, 3        ; Move immediate value 3 into register B
        
        ADD     B           ; Add the contents of register B to register A (A = A + B)
        
        STA     0           ; Store the contents of register A into memory location 'SUM'
        
        HLT                 ; Halt the processor

; --- Data Segment ---
SUM:    DB      0           ; Reserve 1 byte for the result, initialized to 0

        END                 ; End of assembly code
        ";

        let _ = Program::parse(&mut pgm, source);
    }

    #[test]
    fn emit_instructions() {
        let mut pgm = Program::new();

        pgm.sections.push(
            Section {
                name: Some("ABOBA"),
                origin: 0x00,
                data: SectionData::Code(vec![Instruction::ADD(Register::B)]),
            }
        );

        assert_eq!(vec![0x00, 0x00], pgm.emit());
    }
}

