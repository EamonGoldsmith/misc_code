
#[derive(Debug, PartialEq)]
pub enum PseudoInstruction<'a> {
    ORG(u16),
    EQU {label: &'a str, value: &'a str},
    END,
}

impl<'a> PseudoInstruction<'a> {
    pub fn parse(line: &'a str) -> Result<PseudoInstruction<'a>, String> {
        let iter = line.split_whitespace();

        let arguments: Vec<_> = iter
            .into_iter()
            .collect::<Vec<_>>();

        let pseudo = if arguments.len() <= 2 {
            match arguments[0].to_uppercase().as_str() {
                "ORG" => {
                    let address = arguments[1].parse::<u16>()
                        .expect("failed to parse 16-bit address");
                    Ok(PseudoInstruction::ORG(address))
                },
                "END" => Ok(PseudoInstruction::END),
                _ => Err(format!("unrecognised pseudo-instruction: {}", arguments[0])),
            }
        } else if arguments.len() == 3 {
            match arguments[1].to_uppercase().as_str() {
                "EQU" => {
                    Ok(PseudoInstruction::EQU {
                        label: arguments[0],
                        value: arguments[2],
                    })
                },
                _ => Err(format!("unrecognised pseudo-instruction: {}", arguments[1])),
            }
        } else {
            Err(format!("unrecognised pseudo-instruction: {}", line))
        };

        pseudo
    }
}

#[cfg(test)]
mod paseudo_instruction {
    use super::*;

    #[test]
    fn parse_org() {
        assert_eq!(
            PseudoInstruction::parse("ORG   10").unwrap(),
            PseudoInstruction::ORG(10)
        );
    }

    #[test]
    fn parse_equ() {
        assert_eq!(
            PseudoInstruction::parse("aboba   equ   42").unwrap(),
            PseudoInstruction::EQU { label: "aboba", value: "42" }
        );
    }

    #[test]
    fn parse_end() {
        assert_eq!(
            PseudoInstruction::parse("END").unwrap(),
            PseudoInstruction::END
        );
    }

    #[test]
    #[should_panic(expected = "failed to parse 16-bit address: ParseIntError { kind: PosOverflow }")]
    fn parse_invalid_org() {
        let _ = PseudoInstruction::parse("ORG 9999999").unwrap();
    }

    #[test]
    #[should_panic(expected = "unrecognised pseudo-instruction: EQUA")]
    fn parse_invalid_equ() {
        let _ = PseudoInstruction::parse("ABOBA EQUA BLOOB").unwrap();
    }
}

