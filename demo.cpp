#include "./include/helper_functions.h"




int main (int argc, char** argv)
{
    std::cout.setf(std::ios::boolalpha);

    point p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, 
    p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23;

    line l1, l2, l3, l4, l5, l6, l7, l8, l9, l10, l11, l12;

    std::vector<line> kings_crown, triangle, square, square_2, hexagon, 
    polygon_1, polygon_2, polygon_3, polygon_small_square, board_wide_square, small_square;

    std::vector<std::vector<line>> outer_poly, inner_polys;


    p1 = {2, 2};
    p2 = {2, 4};
    p3 = {4, 4};
    p4 = {4, 2};
    p5 = {2, 6};
    p6 = {4, 6};
    p7 = {2, 2};
    p8 = {2, 4};
    p9 = {3, 4};
    p10 = {3, 2};

    l1 = {p2, p3};
    l2 = {p4, p1};
    l3 = {p5, p6};
    l4 = {p7, p8};
    l5 = {p8, p9};
    l6 = {p9, p10};
    l7 = {p10, p7};


    polygon_1.push_back(l1);
    polygon_2.push_back(l3);
    polygon_3.push_back(l2);
    polygon_small_square.push_back(l4);
    polygon_small_square.push_back(l5);
    polygon_small_square.push_back(l6);
    polygon_small_square.push_back(l7);


    p11 = {50, 20};
    p12 = {60, 40};
    p13 = {70, 30};
    p14 = {80, 40};
    p15 = {90, 30};
    p16 = {100, 40};
    p17 = {110, 30};
    p18 = {120, 40};
    p19 = {130, 30};
    p20 = {140, 40};
    p21 = {150, 20};
    p22 = {160, 5};
    p23 = {12, 0};

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
    // l12 = {p22, p23};

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

    inner_polys.push_back(kings_crown);

    p1 = {20, 30};
    p2 = {20, 60};
    p3 = {40, 30};

    l1 = {p1, p2};
    l2 = {p2, p3};
    l3 = {p3, p1};
    triangle.push_back(l1);
    triangle.push_back(l2);
    triangle.push_back(l3);

    inner_polys.push_back(triangle);

    p1 = {50, 50};
    p2 = {50, 75};
    p3 = {75, 75};
    p4 = {75, 50};

    l1 = {p1, p2};
    l2 = {p2, p3};
    l3 = {p3, p4};
    l4 = {p4, p1};
    small_square.push_back(l1);
    small_square.push_back(l2);
    small_square.push_back(l3);
    small_square.push_back(l4); 

    inner_polys.push_back(small_square);


    p1 = {80, 80};
    p2 = {60, 100};
    p3 = {60, 120};
    p4 = {80, 140};
    p5 = {100, 140};
    p6 = {120, 120};
    p7 = {120, 100};
    p8 = {100, 80};

    l1 = {p1, p2};
    l2 = {p2, p3};
    l3 = {p3, p4};
    l4 = {p4, p5};
    l5 = {p5, p6};
    l6 = {p6, p7};
    l7 = {p7, p8};
    l8 = {p8, p1};
    hexagon.push_back(l1);
    hexagon.push_back(l2);
    hexagon.push_back(l3);
    hexagon.push_back(l4); 
    hexagon.push_back(l5);
    hexagon.push_back(l6);
    hexagon.push_back(l7);
    hexagon.push_back(l8);

    inner_polys.push_back(hexagon);


    p1 = {10, 10};
    p2 = {10, 160};
    p3 = {160, 160};
    p4 = {160, 10};

    l1 = {p1, p2};
    l2 = {p2, p3};
    l3 = {p3, p4};
    l4 = {p4, p1};
    board_wide_square.push_back(l1);
    board_wide_square.push_back(l2);
    board_wide_square.push_back(l3);
    board_wide_square.push_back(l4);

    outer_poly.push_back(board_wide_square);


    // Prepare polys
    std::vector<std::vector<std::vector<line>>> original_polys;
    original_polys.push_back(outer_poly);
    original_polys.push_back(inner_polys);

    // Offset by tool thickness
    int mode = EXPAND;
    std::vector<std::vector<line>> offset_polys = offset_total_polys(original_polys, mode);

    // Reorder cuts
    std::vector<std::vector<line>> reordered_polys = offset_polys;
    int offset_value = 3;
    int board_width = 150 + 2*offset_value;
    reorder_final(reordered_polys, board_width); // in-place


    // Plotting
    std::vector<std::vector<line>> plot_polys;
    for (int i = 0; i < outer_poly.size(); i++)
        plot_polys.push_back(outer_poly[i]);
    for (int i = 0; i < inner_polys.size(); i++)
        plot_polys.push_back(inner_polys[i]);
    for (int i = 0; i < reordered_polys.size(); i++)
        plot_polys.push_back(reordered_polys[i]);

    int xlims[2] = {-1, 200};
    int ylims[2] = {-1, 200};
    plot_poly_set(plot_polys, ylims, ylims);


    
}