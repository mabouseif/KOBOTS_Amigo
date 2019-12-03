#include "../helper_functions.h"

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
    point p7 = {2, 2};
    point p8 = {2, 4};
    point p9 = {3, 4};
    point p10 = {3, 2};

    line l1 = {p2, p3};
    line l2 = {p4, p1};
    line l3 = {p5, p6};

    line l4 = {p7, p8};
    line l5 = {p8, p9};
    line l6 = {p9, p10};
    line l7 = {p10, p7};

    std::vector<std::vector<line>> stacked_horizontals;
    std::vector<std::vector<line>> stacked_horizontals_w_square;

    std::vector<line> polygon_1;
    std::vector<line> polygon_2;
    std::vector<line> polygon_3;
    std::vector<line> polygon_small_square;
    

    polygon_1.push_back(l1);
    polygon_2.push_back(l2);
    polygon_3.push_back(l3);
    polygon_small_square.push_back(l4);
    polygon_small_square.push_back(l5);
    polygon_small_square.push_back(l6);
    polygon_small_square.push_back(l7);

    

    stacked_horizontals.push_back(polygon_1);
    stacked_horizontals.push_back(polygon_2);
    stacked_horizontals.push_back(polygon_3);

    stacked_horizontals_w_square.push_back(polygon_1);
    stacked_horizontals_w_square.push_back(polygon_2);
    stacked_horizontals_w_square.push_back(polygon_small_square);
    stacked_horizontals_w_square.push_back(polygon_3);


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
    line l8 = {p18, p19};
    line l9 = {p19, p20};
    line l10 = {p20, p21};
    line l11 = {p21, p11};
    // line l12 = {p22, p23};

    std::vector<line> kings_crown;
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

    std::vector<line> triangle;
    p1 = {2, 7};
    p2 = {2, 9};
    p3 = {4, 7};

    l1 = {p1, p2};
    l2 = {p2, p3};
    l3 = {p3, p1};
    triangle.push_back(l1);
    triangle.push_back(l2);
    triangle.push_back(l3);

    stacked_horizontals_w_square.push_back(kings_crown);

    // stacked_horizontals_w_square.clear();
    // stacked_horizontals_w_square.push_back(triangle);
    

    std::cout << "poly set size before: " << stacked_horizontals_w_square.size() << std::endl;

    int board_width = 2;
    std::map<int, custom_arr_type> my_map = board_wide_poly(stacked_horizontals_w_square, board_width);
    std::cout << "Number of polygons with a line that is board-wide: " << my_map.size() << std::endl;
    std::cout << "poly set size after: " << stacked_horizontals_w_square.size() << std::endl;


    for(auto const &pair: my_map)
        std::cout << pair.first << std::endl;

    // https://stackoverflow.com/questions/44190962/using-array-as-map-key-impossible-even-with-custom-allocator

    int xlims[2] = {-1, 10};
    int ylims[2] = {-1, 10};
    plot_poly_set(stacked_horizontals_w_square, ylims, ylims);






    return 0;
}