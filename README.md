# PandasPlusPlus

I am writing this as a native library for C++ that has utilities similar to 
python's Pandas library. I really like the pandas library, and I thought that
it's utility combined with C++'s fast speeds could prove interesting for data
processing at a lower level

This is just a pet project at the moment and definitely has some kinks and 
wrinkles that can be ironed out and optimized

## Known Issues
the use of std::views::zip in the matrix addition operator does not seem to be
supported yet by clang, but is by msvc and gcc
