run this command to compile and run:
```
g++ -std=c++11 ../helper_functions.cpp ordering_test.cpp -o ordering_test && ./ordering_test
```
or if you want to only run: 
```
./ordering_test
```

for the stacked board-wide polys test:
```
g++ ../helper_functions.cpp test_stacked_horizontals.cpp -o test_stacked_horizontals -std=c++11 -I/usr/include/python2.7 -lpython2.7 && ./test_stacked_horizontals
```
or if you want to only run:
```
./test_stacked_horizontals
```

For running valgrind memory-leak and other checks (comment out plotting matplotlib-based plotting function):
```
g++ -g ../helper_functions.cpp test_stacked_horizontals.cpp -o test_stacked_horizontals -std=c++11 -I/usr/include/python2.7 -lpython2.7 && valgrind --leak-check=yes --track-origins=yes --dsymutil=yes ./test_stacked_horizontals
```


## TODO
- Board-wide polys
- Direction of final line

## Notes
