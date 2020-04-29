use std::io;
use std::io::prelude::*;
use std::io::BufReader;

use crate::lexer::Lexer;
use crate::parser::parse;

static PROMPT: &str = ">>";

pub struct Repl<O: Write> {
    stdout: O,
    partial_output: bool,
}

impl<O: Write> Repl<O> {
    pub fn run<I: Read>(stdin: I, stdout: O) -> io::Result<()> {
        let mut repl = Repl {
            stdout,
            partial_output: false,
        };
        repl.init()?;
        for input in BufReader::new(stdin).lines() {
            repl.execute(input?)?;
        }
        repl.cleanup()?;
        Ok(())
    }

    fn init(&mut self) -> io::Result<()> {
        writeln!(self.stdout, "rust/mnky 1.0")?;
        self.prompt()?;
        Ok(())
    }

    fn execute(&mut self, input: String) -> io::Result<()> {
        self.partial_output = false;
        for tok in Lexer::new(&input) {
            println!("{:?}", tok);
        }
        parse(Lexer::new(&input));
        self.prompt()?;

        Ok(())
    }

    fn cleanup(&mut self) -> io::Result<()> {
        if self.partial_output {
            writeln!(self.stdout)?;
        }
        Ok(())
    }

    fn prompt(&mut self) -> io::Result<()> {
        self.partial_output = true;
        write!(self.stdout, "{} ", PROMPT)?;
        self.stdout.flush()
    }
}
