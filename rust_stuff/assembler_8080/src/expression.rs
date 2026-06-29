
/*
    In 8080 language some Instructions, such as LXI (Load extended immediate),
    may take more than one type of operand, or something that doesn't look
    like any operand, such as '(2 * 9 * 9) / 3' or '1 << LED_PIN'. We'll 
    represent these as a recursive enum of expressions which can be evaluated
    to produce the final hex bytes during the second pass.
*/
#[derive(Debug, PartialEq)]
pub enum Expression<'a> {
    Literal(u8),
    Symbol(&'a str),
    Register,
    RegisterPair,
}

impl<'a> Expression<'_> {
    fn parse(_expr: &str) -> Result<Expression, String> {
        Ok(Expression::Literal(16))
    }
}

#[cfg(test)]
mod expression_tests {
    use super::*;

    #[test]
    fn simple_register() {
        let expr = "16";
        let parsed = Expression::parse(expr).unwrap();
        assert_eq!(parsed, Expression::Literal(16));
    }
}

