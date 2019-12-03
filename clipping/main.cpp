#include "../helper_functions.h"


using namespace ClipperLib;




/*
A note about scaling:
Because ClipperOffset uses integer coordinates, you have to scale coordinates to maintain precision and make arcs smooth - also in the case of integer input.
Javascript Clipper provides four functions for this purpose: ScaleUpPath, ScaleUpPaths, ScaleDownPath and ScaleDownPaths.
Scaling is highly recommended if JoinType is jtRound or EndType is etRound.
*/

/*
When offsetting closed paths (polygons), it's important that:
their orientations are consistent such that outer polygons share the same orientation while holes have the opposite orientation
they do not self-intersect.
*/


// Modify polygon to new point vals
// Note: Clipper returns an ordered sequence of points.
// Need to check the open-polygon case.   


// Need to handle the open-polygon case here





//////////////////////////////////////////////////////////////////////
////////////////////////////// Main //////////////////////////////////
//////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    // This is here because "true" and "false" are printed out as 1 and 0, whereas this fixes it.
    std::cout.setf(std::ios::boolalpha);


    std::vector<line> kings_crown;

    point p11 = {10, 10};
    point p12 = {10, 50};
    point p13 = {20, 30};
    point p14 = {30, 50};
    point p15 = {40, 30};
    point p16 = {50, 50};
    point p17 = {60, 30};
    point p18 = {70, 50};
    point p19 = {80, 30};
    point p20 = {90, 50};
    point p21 = {100, 10};
    point p22 = {110, 50};
    point p23 = {120, 10};

    line l1 = {p11, p12};
    line l2 = {p12, p13};
    line l3 = {p13, p14};
    line l4 = {p14, p15};
    line l5 = {p15, p16};
    line l6 = {p16, p17};
    line l7 = {p17, p18};
    line l8 = {p18, p19};
    line l9 = {p19, p20};
    line l10 = {p20, p21};
    line l11 = {p21, p22};

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

    std::vector<std::vector<line>> poly_set;
    std::vector<line> reordered = reorder_cuts(kings_crown);
    poly_set.push_back(reordered);

    int xlims[2] = {0, 120};
    int ylims[2] = {0, 120};
    plot_poly_set(poly_set, xlims, ylims);


    // std::vector<line> square;

    // point p1 = {10, 10};
    // point p2 = {10, 70};
    // point p3 = {70, 70};
    // point p4 = {70, 10};
    // line l1 = {p1, p2};
    // line l2 = {p2, p3};
    // line l3 = {p3, p4};
    // line l4 = {p4, p1};
    // square.push_back(l1);
    // square.push_back(l2);
    // square.push_back(l3);
    // square.push_back(l4);


    // std::vector<line> square_2;
    // p1 = {25, 25};
    // p2 = {25, 35};
    // p3 = {35, 35};
    // p4 = {35, 25};
    // l1 = {p1, p2};
    // l2 = {p2, p3};
    // l3 = {p3, p4};
    // l4 = {p4, p1};
    // square_2.push_back(l1);
    // square_2.push_back(l2);
    // square_2.push_back(l3);
    // square_2.push_back(l4);


    // std::vector<line> triangle;

    // p1 = {40, 40};
    // p2 = {50, 50};
    // p3 = {60, 40};
    // l1 = {p1, p2};
    // l2 = {p2, p3};
    // l3 = {p3, p1};
    // triangle.push_back(l1);
    // triangle.push_back(l2);
    // triangle.push_back(l3);


    // std::vector<std::vector<std::vector<line>>> total_polys;
    // std::vector<std::vector<line>> poly_set;

    // std::vector<line> polygon = square;
    // poly_set.push_back(polygon);
    // total_polys.push_back(poly_set);

    // poly_set.clear();
    // poly_set.push_back(triangle);
    // poly_set.push_back(square_2);
    // total_polys.push_back(poly_set);


    // std::vector<std::vector<line>> modified_poly_set = offset_total_polys(total_polys, EXPAND);
    
    // for(int i = 0; i < modified_poly_set.size(); i++)
    //     modified_poly_set[i] = reorder_cuts(modified_poly_set[i]);


    // modified_poly_set.push_back(square);
    // modified_poly_set.push_back(square_2);
    // modified_poly_set.push_back(triangle);

    // int xlims[2] = {0, 120};
    // int ylims[2] = {0, 120};
    // plot_poly_set(modified_poly_set, xlims, ylims);


    return 0;

}