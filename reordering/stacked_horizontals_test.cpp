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
    

    std::cout << "poly set size before: " << stacked_horizontals_w_square.size() << std::endl;
    

    int board_width = 2;
    std::map<int, custom_arr_type> my_map = board_wide_poly(stacked_horizontals_w_square, board_width);
    std::cout << "Number of polygons with a line that is board-wide: " << my_map.size() << std::endl;
    std::cout << "poly set size after: " << stacked_horizontals_w_square.size() << std::endl;

    for(auto const &pair: my_map)
        std::cout << pair.first << std::endl;

    // https://stackoverflow.com/questions/44190962/using-array-as-map-key-impossible-even-with-custom-allocator

    plot_poly_set(stacked_horizontals_w_square);


    return 0;
}