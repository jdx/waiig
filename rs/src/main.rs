mod lexer;
mod token;
mod repl;

use repl::Repl;
use std::io;

fn main() -> Result<(), std::io::Error> {
    Repl::run(io::stdin(), io::stdout())
}
