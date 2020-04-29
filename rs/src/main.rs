mod lexer;
mod token;
mod repl;

use repl::Repl;
use std::io;

fn main() -> io::Result<()> {
    Repl::run(io::stdin(), io::stdout())
}
