# PandasPlusPlus

I am writing this as a native library for C++ that has utilities similar to 
python's Pandas library. I really like the pandas library, and I thought that
it's utility combined with C++'s fast speeds could prove interesting for data
processing at a lower level

This is just a pet project at the moment and definitely has some kinks and 
wrinkles that can be ironed out and optimized

## Known Issues
The files Dataframe.hpp and Exceptions.hpp should be considered legacy and no
longer supported. They were written a long time ago and are being re-written
in C++20 from scratch. Matrix.hpp is where continuing work will be done until
Dataframe and Exceptions are safe to delete.

## Plans (Is this fleshed out enough to call a roadmap? Probably not)
* Exported C APIs so that the ppp template library can be imported with the C
ABI from other languages.
* Support to be called/used directly from python3. Plan to implement directly
using the python3 C-API from the c bindings mentioned above.
* Support for direct utilization from dotnet languages. Planning on using
C++/CLI to create an API with managed resources dotnet can understand
