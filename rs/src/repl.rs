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
            writeln!(self.stdout, "{:?}", tok)?;
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

#[cfg(test)]
mod tests {
    use super::*;
    use pretty_assertions::{assert_eq};
    use std::io::Cursor;

    #[test]
    fn test_repl() {
        assert_eq!(exec("foo"), "rust/mnky 1.0
>> Token { type_: Ident, literal: \"foo\", row: 0, col: 1 }
>> \n");
    }

    fn exec(input: &str) -> String {
        let stdin = Cursor::new(input);
        let mut stdout = Cursor::new(Vec::new());
        Repl::run(stdin, &mut stdout).unwrap();

        return String::from_utf8(stdout.into_inner()).unwrap();
    }
}
