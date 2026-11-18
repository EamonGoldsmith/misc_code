extern crate command;

use std::env;
use std::process::ExitCode;
use command::{command, command_list};

struct Command {
    name: &'static str,
    description: &'static str,
    run: fn(&str, env::Args) -> ExitCode,
}

fn usage(program: &str, _args: env::Args) {
    eprintln!("Usage: {program} <command> [input]");
    eprintln!("Commands:");
    for command in COMMANDS.iter() {
        eprintln!("     {name} - {description}",
            name = command.name, description = command.description);
    }
}

#[command("hello", "prints hello world")]
fn hello_command(_program: &str, _args: env::Args) -> ExitCode {
    println!("Hello, World!");

    ExitCode::SUCCESS
}

#[command("reverse", "prints reversed input")]
fn reverse_command(_program: &str, args: env::Args) -> ExitCode {
    for arg in args {
        println!("{}", arg.chars().rev().collect::<String>());
    }

    ExitCode::SUCCESS
}

#[command("uppercase", "prints the uppercase of the input")]
fn uppercase_command(_program: &str, args: env::Args) -> ExitCode {
    for arg in args {
        println!("{}", arg.to_uppercase());
    }

    ExitCode::SUCCESS
}

#[command("help", "print this help message")]
fn help_command(program: &str, mut args: env::Args) -> ExitCode {
    if let Some(name) = args.next() {
        if let Some (command) = COMMANDS.iter().find(|command| command.name == name) {
            println!("{name} - {description}",
                name = command.name, description = command.description);
        }
    } else {
        usage(program, args);
    }

    ExitCode::SUCCESS
}

const COMMANDS: &[Command] = command_list!();

fn main() -> ExitCode { 
    let mut args = env::args();
    let program = args.next().expect("program");

    if let Some(command_name) = args.next() {
        if let Some(command) = COMMANDS.iter().find(|command| command.name == command_name) {
            return (command.run)(&program, args);
        } else {
            usage(&program, args);
            eprintln!("ERROR: command {command_name} is not found");
            return ExitCode::FAILURE;
        }
    } else {
        usage(&program, args);
        eprintln!("ERROR: no command is provided");
        return ExitCode::FAILURE;
    }
    
    ExitCode::SUCCESS
}
