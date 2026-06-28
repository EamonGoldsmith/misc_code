enum Register {
    A, B, C, D, E, H, L, M
}

enum Instructions {
    ORG(u16),
    //EQU {label: &str, value: },

    MVI(u16),
    MOV(Register, Register),
    OUT(u16),
    CALL(&str),
    ADD(,
    STA(Register, u16),
    JMP(Register),
    LXI(Register, u16),
    DCX,
    ORA,
    JNZ,

    RET,
    HLT,
    END,
}

pub fn parse_line(
    mnemonic: &str,
    first: Option<&str>,
    second: Option<&str>
) -> Result<Instruction, String> {

}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_mov() {
    }

    #[test]
    fn parse_add() {
        let parsed = Instruction::ADD;
        let line = "ADD     B           ; Add register B to register A";

        assert_eq!(parsed, parse_line(line));
    }

    fn parse_org() {
        todo!();
    }
}

