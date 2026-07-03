
#[derive(Debug, PartialEq)]
pub enum Register {
    B, 
    C,
    D,
    E,
    H, // High 
    L, // Low
    M,
    A, // Accumulator
}

impl<'a> Register {
    pub fn parse(name: &str) -> Result<Register, String> {
        let register = match name.to_uppercase().as_str() {
            "A" => Register::A,
            "B" => Register::B,
            "C" => Register::C,
            "D" => Register::D,
            "E" => Register::E,
            "H" => Register::H,
            "L" => Register::L,
            _ => return Err(format!("unrecognised register: {}", name)),
        };

        Ok(register)
    }

    pub fn emit(&self) -> u8 {
        match self {
            Register::B => 0b000u8,
            Register::C => 0b001u8,
            Register::D => 0b010u8,
            Register::E => 0b011u8,
            Register::H => 0b100u8,
            Register::L => 0b101u8,
            Register::M => 0b110u8,
            Register::A => 0b111u8,
        }
    }
}

#[derive(Debug, PartialEq)]
pub enum RegisterPair {
    B,      // B and C pair
    D,      // D and E pair
    H,      // High and low pair
    SP,     // Stack Pointer
    PSW,    // Accumulator and status flags
}

impl RegisterPair {
    pub fn parse<'a>(name: &str) -> Result<RegisterPair, String> {
        let pair = match name.to_uppercase().as_str() {
            "B" => RegisterPair::B,
            "D" => RegisterPair::D,
            "H" => RegisterPair::H,
            "SP" => RegisterPair::SP,
            "PSW" => RegisterPair::PSW,
            _ => return Err(format!("unrecognised register pair: {}", name)),
        };

        Ok(pair)
    }

    pub fn emit(&self) -> u8 {
        match self {
            RegisterPair::B => 0b00,
            RegisterPair::D => 0b01,
            RegisterPair::H => 0b10,
            RegisterPair::SP => 0b11,
            RegisterPair::PSW => 0b11,
        }
    }
}
