#include "helper_functions.h"


int main(int argc, char** argv)
{

    // This is here because "true" and "false" are printed out as 1 and 0, whereas this fixes it.
    std::cout.setf(std::ios::boolalpha);

    point p1 = {2, 2};
    point p2 = {2, 4};
    point p3 = {4, 4};
    point p4 = {4, 2};
    point p5 = {2, 6};
    point p6 = {4, 6};

    line l1 = {p2, p3};
    line l2 = {p4, p1};
    line l3 = {p5, p6};

    std::vector<std::vector<line>> stacked_horizontals;

    std::vector<line> polygon_1;
    std::vector<line> polygon_2;
    std::vector<line> polygon_3;

    polygon_1.push_back(l1);
    polygon_2.push_back(l2);
    polygon_3.push_back(l3);

    stacked_horizontals.push_back(polygon_1);
    stacked_horizontals.push_back(polygon_2);
    stacked_horizontals.push_back(polygon_3);


    return 0;
}