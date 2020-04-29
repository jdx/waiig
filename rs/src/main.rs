mod lexer;
mod token;
mod repl;
mod parser;

use repl::Repl;
use std::io;

fn main() -> io::Result<()> {
    Repl::run(io::stdin(), io::stdout())
}
