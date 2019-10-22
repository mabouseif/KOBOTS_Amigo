#include "ordering_function.h"


int main(int argc, char** agrv)
{

    std::vector<line> square;
    std::vector<line> triangle;
    std::vector<line> kings_crown;
    std::vector<line> upper_left_corner_mirrored_L_shape;
    std::vector<line> lower_left_corner_inverted_L_shape;
    std::vector<line> lower_left_corner_inverted_wide_angle_L_shape;

    point p1 = {2, 2};
    point p2 = {2, 4};
    point p3 = {4, 4};
    point p4 = {4, 2};
    // point p5 = {7, 8};

    line l1 = {p1, p2};
    line l2 = {p2, p3};
    line l3 = {p3, p4};
    line l4 = {p4, p1};
    line l5;
    line l6;
    line l7;
    line l8;
    line l9;
    line l10;
    line l11;
    line l12;

    square.push_back(l1);
    square.push_back(l2);
    square.push_back(l3);
    square.push_back(l4);

    l1 = {p1, p3};
    l2 = {p3, p4};
    l3 = {p4, p1};

    triangle.push_back(l1);
    triangle.push_back(l2);
    triangle.push_back(l3);

    point p11 = {0, 0};
    point p12 = {1, 5};
    point p13 = {2, 3};
    point p14 = {3, 5};
    point p15 = {4, 3};
    point p16 = {5, 5};
    point p17 = {6, 3};
    point p18 = {7, 5};
    point p19 = {8, 3};
    point p20 = {9, 5};
    point p21 = {10, 0};
    point p22 = {11, 5};
    point p23 = {12, 0};

    l1 = {p11, p12};
    l2 = {p12, p13};
    l3 = {p13, p14};
    l4 = {p14, p15};
    l5 = {p15, p16};
    l6 = {p16, p17};
    l7 = {p17, p18};
    l8 = {p18, p19};
    l9 = {p19, p20};
    l10 = {p20, p21};
    l11 = {p21, p11};
    l12 = {p22, p23};

    kings_crown.push_back(l1);
    kings_crown.push_back(l2);
    kings_crown.push_back(l3);
    kings_crown.push_back(l4);
    kings_crown.push_back(l5);
    kings_crown.push_back(l6);
    kings_crown.push_back(l7);
    kings_crown.push_back(l8);
    kings_crown.push_back(l9);
    kings_crown.push_back(l10);
    kings_crown.push_back(l11);
    // kings_crown.push_back(l12);

    p1 = {0, 5};
    p2 = {2, 5};
    p3 = {2, 8};

    l1 = {p1, p2};
    l2 = {p2, p3};

    upper_left_corner_mirrored_L_shape.push_back(l1);
    upper_left_corner_mirrored_L_shape.push_back(l2);

    p1 = {2, 0};
    p2 = {2, 4};
    p3 = {0, 4};

    l1 = {p1, p2};
    l2 = {p2, p3};
    
    lower_left_corner_inverted_L_shape.push_back(l1);
    lower_left_corner_inverted_L_shape.push_back(l2);


    p1 = {2, 0};
    p2 = {2, 4};
    p3 = {0, 5};

    l1 = {p1, p2};
    l2 = {p2, p3};
    
    lower_left_corner_inverted_wide_angle_L_shape.push_back(l1);
    lower_left_corner_inverted_wide_angle_L_shape.push_back(l2);


    std::cout << "Running.." << std::endl;


    std::vector<line> reorderd_sequence = reorder_cuts(square);


    return 0;
}

