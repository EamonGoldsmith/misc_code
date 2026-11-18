extern crate proc_macro;
use proc_macro::*;

use std::iter::FromIterator;

#[derive(Debug)]
struct Command {
    name: String,
    description: String,
    run: String,
}

static mut COMMANDS: Vec<Command> = Vec::new();

fn expect_literal(lex: &mut impl Iterator<Item=TokenTree>) -> Literal {
    match lex.next() {
        Some(TokenTree::Literal(literal)) => literal,
        Some(_token) => panic!("expected literal token"),
        None => panic!("expected literal but got nothing"),
    }
}

fn expect_punct(lex: &mut impl Iterator<Item=TokenTree>, ch: char) -> Punct {
    match lex.next() {
        Some(TokenTree::Punct(punct)) => if punct.as_char() == ch {
            punct
        } else {
            panic!("Expected punct '{expected}', but got '{actual}'",
                expected = ch, actual = punct.as_char())
        },
        Some(_token) => panic!("expected punct token"),
        None => panic!("expected punct but got nothing"),
    }
}

fn expect_ident(lex: &mut impl Iterator<Item=TokenTree>) -> Ident {
    match lex.next() {
        Some(TokenTree::Ident(ident)) => ident,
        Some(_token) => panic!("expected ident token"),
        None => panic!("expected ident but got nothing"),
    }
}

#[proc_macro_attribute]
pub fn command(args: TokenStream, input: TokenStream) -> TokenStream {
    let mut args_iter = args.into_iter();

    let name = expect_literal(&mut args_iter).to_string();
    let comma = expect_punct(&mut args_iter, ',');
    let description = expect_literal(&mut args_iter).to_string();
    
    let mut input_iter = input.clone().into_iter();
    let _ = expect_ident(&mut input_iter);
    let run = expect_ident(&mut input_iter).to_string();

    unsafe {
        COMMANDS.push(Command { name, description, run })
    }

    input
}

fn command_fields(command: &Command) -> TokenStream {
    let mut tokens: Vec<TokenTree> = Vec::new();
    
    tokens.push(Ident::new("name", Span::call_site()).into());
    tokens.push(Punct::new(':', Spacing::Alone).into());
    tokens.push(command.name.parse::<Literal>().unwrap().into());
    tokens.push(Punct::new(',', Spacing::Alone).into());

    tokens.push(Ident::new("description", Span::call_site()).into());
    tokens.push(Punct::new(':', Spacing::Alone).into());
    tokens.push(command.description.parse::<Literal>().unwrap().into());
    tokens.push(Punct::new(',', Spacing::Alone).into());

    tokens.push(Ident::new("run", Span::call_site()).into());
    tokens.push(Punct::new(':', Spacing::Alone).into());
    tokens.push(Ident::new(&command.run, Span::call_site()).into());
    tokens.push(Punct::new(',', Spacing::Alone).into());
    
    TokenStream::from_iter(tokens)
}

fn render_commands() -> TokenStream {
    let mut tokens: Vec<TokenTree> = Vec::new();

    unsafe {
        for command in COMMANDS.iter() {
            tokens.push(Ident::new("Command", Span::call_site()).into());
            tokens.push(Group::new(Delimiter::Brace, command_fields(&command)).into());
            tokens.push(Punct::new(',', Spacing::Alone).into());
        }
    }

    TokenStream::from_iter(tokens)
}

#[proc_macro]
pub fn command_list(_item: TokenStream) -> TokenStream {
    let stream = TokenStream::from_iter([
        TokenTree::Punct(Punct::new('&', Spacing::Joint)),
        TokenTree::Group(Group::new(Delimiter::Bracket, render_commands())),
    ]);
    
    stream
}
