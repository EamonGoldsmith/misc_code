use std::{
    env,
    fs,
    process,
    error::Error,
};

fn print_usage() {
    println!("idk");
}

fn main() {
    let config = Config::build(env::args()).unwrap_or_else(|err| {
        eprintln!("Incorrect arguments: {err}");
        print_usage();
        process::exit(1);
    });

    println!("Assembling: {}", config.input);
    println!("Into: {}", config.output);

    if let Err(e) = run(config) {
        eprintln!("Assembler error: {e}");
        process::exit(1);
    }
}

fn run(config: Config) -> Result<(), Box<dyn Error>> {
    let contents = fs::read_to_string(config.input)?;


    Ok(())
}

pub struct Config {
    pub input: String,
    pub output: String,
}

impl Config {
    fn build(
        mut args: impl Iterator<Item = String>,
    ) -> Result<Config, &'static str> {
        args.next();

        let input = match args.next() {
            Some(arg) => arg.to_string(),
            None => return Err("Didn't get an input file"),
        };

        // output out.hex if no name is provided
        let output = match args.next() {
            Some(arg) => arg.to_string(),
            None => "out.hex".to_string(),
        };
        
        Ok(Config {
            input,
            output,
        })
    }
}
