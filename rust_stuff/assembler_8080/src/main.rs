use std::{ env, fs, process, error::Error };
use assembler_8080::assemble;

fn print_usage() {
    println!("usage: ./as input.s output.hex");
}

fn main() {
    let config = Config::build(env::args()).unwrap_or_else(|err| {
        eprintln!("incorrect arguments: {err}");
        print_usage();
        process::exit(1);
    });

    println!("assembling: {}", config.input);
    println!("into: {}", config.output);

    if let Err(e) = run(config) {
        eprintln!("assembler error: {e}");
        process::exit(1);
    }
}

fn run(config: Config) -> Result<(), Box<dyn Error>> {
    let contents = fs::read_to_string(config.input)?;
    
    if let Ok(hex) = assemble(&contents) {
        println!("{:?}", hex);
    };



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
            None => return Err("no input files"),
        };

        // Output out.hex if no name is provided
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
