run this command to compile and run:
```
g++ -std=c++11 ../include/clipper.cpp ../include/helper_functions.cpp -I/usr/include/python2.7 -lpython2.7 ordering_test.cpp -o ordering_test && ./ordering_test
```
or if you want to only run: 
```
./ordering_test
```

for the stacked board-wide polys test:
```
g++ ../include/helper_functions.cpp ../include/clipper.cpp stacked_horizontals_test.cpp -o stacked_horizontals_test -std=c++11 -I/usr/include/python2.7 -lpython2.7 && ./stacked_horizontals_test
```
or if you want to only run:
```
./stacked_horizontals_test
```

For running valgrind memory-leak and other checks (comment out plotting matplotlib-based plotting function):
```
g++ -g ../include/helper_functions.cpp ../include/clipper.cpp stacked_horizontals_test.cpp -o stacked_horizontals_test -std=c++11 -I/usr/include/python2.7 -lpython2.7 && valgrind --leak-check=yes --track-origins=yes --dsymutil=yes ./stacked_horizontals_test
```


## TODO
- Board-wide polys
- Direction of final line

## Notes
