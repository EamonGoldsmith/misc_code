pub mod register;
pub mod expression;
pub mod instruction;
pub mod pseudo_instruction;
pub mod section;
pub mod program;

use crate::program::Program;

pub fn assemble(contents: &str) -> Result<(), String> {

    let mut pgm = Program::new();

    // TODO: do something with this
    let _err = pgm.parse(contents);

    Ok(())
}
