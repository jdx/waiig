use std::io;
use std::io::prelude::*;
use std::io::BufReader;

use crate::lexer::Lexer;

static PROMPT: &str = ">>";

pub struct Repl<O: Write> {
    stdout: O,
}

impl<O: Write> Repl<O> {
    pub fn run<I: Read>(stdin: I, stdout: O) -> Result<(), io::Error> {
        println!("rust/mnky 1.0");

        let mut repl = Repl{stdout};
        repl._run(stdin)
    }

    fn _run<I: Read>(&mut self, stdin: I) -> Result<(), io::Error> {
        self.prompt()?;
        for input in BufReader::new(stdin).lines() {
            let input = input?;
            for tok in Lexer::new(&input) {
                println!("{:?}", tok);
            }
            self.prompt()?;
        }
        println!();

        Ok(())
    }

    fn prompt(&mut self) -> Result<(), io::Error> {
        write!(self.stdout, "{} ", PROMPT)?;
        self.stdout.flush()
    }
}
