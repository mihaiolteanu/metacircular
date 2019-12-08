This is a Scheme interpreter written in C. The name of the project comes from
SICP's [metacircular evaluator](https://mitpress.mit.edu/sites/default/files/sicp/full-text/sicp/book/node76.html). Of course, being written in C means it is no
longer metacircular, but the approach in developing the interpreter is taken
from SICP's eval/apply loop. The main motivation for writing and studying this
project is in better understanding SICP, Scheme and interpreters/compilers in
general. Regarded as such, this project is/was a success.

Another additional motivation for starting this project was that of using and
playing with [modern CMake](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/). And if you're starting a new C project that will
suffer a lot of bug-fixing and redesigning, as writing a compiler or interpreter
will surely entail, why not use a mocking library ([cmock](http://www.throwtheswitch.org/cmock)) to generate test
mocks and a testing library ([unity](http://www.throwtheswitch.org/unity)) to run all the tests in one go? So this
project is also an example on how to set these up and used them so that you can
code with more confidence.
