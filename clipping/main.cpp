#include "clipper.hpp"  
#include "clipper_console_demo.cpp"
#include "../helper_functions.h"
#include "../matplotlibcpp.h"

using namespace ClipperLib;

#define SHRINK 0
#define EXPAND 1


std::vector<line> offset_poly(std::vector<line> polygon, int mode)
{
    int offset_val;

    switch(mode)
    {
        case(0): 
            std::cout << "Shrinking polygon by offset" << std::endl;
            offset_val = -3;
            break;
        
        case(1):
            std::cout << "Expanding polygon by offset" << std::endl;
            offset_val = 3;
            break;

        default:
            std::cout << "Invalid offset mode!" << std::endl;
            exit (EXIT_FAILURE);
    }
    

    // ClipperLib-related variables
    Path subj;
    Paths original;
    Paths solution;

    // Convert to ClipperLib format
    for(int i = 0; i < polygon.size(); i++)
    {
        subj << IntPoint(polygon[i].p1.x, polygon[i].p1.y);
        subj << IntPoint(polygon[i].p2.x, polygon[i].p2.y);
    }

    std::cout << "subj size pre-clean: " << subj.size() << std::endl;

    CleanPolygon(subj);

    std::cout << "subj size post-clean: " << subj.size() << std::endl;


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


    // Offset using passed args
    double miter_limit = 6.0;
    std::cout << "Miter limit = " << miter_limit << std::endl;

    ClipperOffset co;    
    co.MiterLimit = miter_limit;
    co.AddPath(subj, jtMiter, etClosedPolygon);
    co.Execute(solution, offset_val);

    // Modify polygon to new point vals
    // Note: Clipper returns an ordered sequence of points.
    // Need to check the open-polygon case.
    line temp_line;
    int n = solution[0].size();

    if (n==0)
    {
      std::cout << "In offset function, solution size is 0" << std::endl;
      EXIT_FAILURE;
    }
    
    // Need to handle the open-polygon case here
    bool poly_or_not = is_polygon(polygon);
    int z = n;

    std::vector<line> new_polygon;
    for(int i = 0; i < z; i++)
    {
        std::cout << "mod num" << (i+1)%n << std::endl;
        temp_line.p1.x = solution[0][i].X;
        temp_line.p1.y = solution[0][i].Y;
        temp_line.p2.x = solution[0][(i+1)%n].X;
        temp_line.p2.y = solution[0][(i+1)%n].Y; // using the modulu operator

        new_polygon.push_back(temp_line);
    }



    // Visual (numeric) content check
    for(int i = 0; i < solution[0].size(); i++)
    {
      std::cout << "(" << solution[0][i].X<< ", " << solution[0][i].Y << ")" << std::endl;
      std::cout << "(" << polygon[i].p1.x << ", " << polygon[i].p1.y << ")\n" << std::endl;
      std::cout << std::endl;
    }

    return new_polygon;


    // //let's see the result too ...
    // original << subj;

    // SVGBuilder svg;
    // svg.style.penWidth = 1.5;
    // svg.style.brushClr = 0x1200009C;
    // svg.style.penClr = 0xFF000000;
    // svg.style.pft = pftNonZero;
    // svg.AddPaths(original);
    // // svg.style.brushClr = 0x129C0000;
    // // svg.style.penClr = 0xCCFFA07A;
    // // svg.style.pft = pftNonZero;
    // // svg.AddPaths(clip);
    // svg.style.brushClr = 0x3000FF00;
    // svg.style.penClr = 0xFF006600;
    // svg.style.pft = pftNonZero;
    // svg.AddPaths(solution);
    // svg.SaveToFile("solution_main.svg");//, svg_scale);

    // //finally, show the svg image in the default viewing application (not working)
    // system("./solution_main.svg"); 


    // return new_polygon;

}


////////////////////////////////////////////////////////////////////
std::vector<std::vector<line>> offset_total_polys(std::vector<std::vector<std::vector<line>>> total_polys)
{
    std::vector<std::vector<line>> modified_total_polys;
    std::vector<line> outer_poly = total_polys[0][0];
    std::vector<vector<line>> hole_polys = total_polys[1];


    // modified_total_polys.push_back(offset_poly(outer_poly, EXPAND));

    // if (hole_polys.size() > 0)
    //     for(int i = 0; i < hole_polys.size(); i++)
    //         modified_total_polys.push_back(offset_poly(hole_polys[i], SHRINK));

    // return modified_total_polys;


    Path subj_outer;
    Path subj_inner;
    Paths subj_inner_all;
    Paths original_outer;
    Paths solution;

    // Convert to ClipperLib format
    for(int i = 0; i < outer_poly.size(); i++)
    {
        subj_outer << IntPoint(outer_poly[i].p1.x, outer_poly[i].p1.y);
        subj_outer << IntPoint(outer_poly[i].p2.x, outer_poly[i].p2.y);
    }
    
    CleanPolygon(subj_outer);

    for(int i = 0; i < hole_polys.size(); i++)
    {
        for(int j = 0; j < hole_polys[i].size(); j++)
        {
            subj_inner << IntPoint(hole_polys[i][j].p1.x, hole_polys[i][j].p1.y);
            subj_inner << IntPoint(hole_polys[i][j].p2.x, hole_polys[i][j].p2.y);
        }

        CleanPolygon(subj_inner);
        subj_inner_all << subj_inner;
    }

    ReversePaths(subj_inner_all);

    double miter_limit = 6.0;
    std::cout << "Miter limit = " << miter_limit << std::endl;

    ClipperOffset co;
    int offset_val = 3;
    co.MiterLimit = miter_limit;
    co.AddPath(subj_outer, jtMiter, etClosedPolygon);
    co.AddPaths(subj_inner_all, jtMiter, etClosedPolygon);
    co.Execute(solution, offset_val);

    std::cout << "Solution size: " << solution[0].size() << std::endl;


    //let's see the result too ...
    original_outer << subj_outer;

    SVGBuilder svg;
    svg.style.penWidth = 1.5;
    svg.style.brushClr = 0x1200009C;
    svg.style.penClr = 0xFF000000;
    svg.style.pft = pftNonZero;
    svg.AddPaths(original_outer);

    svg.style.brushClr = 0x1200009C;
    svg.style.penClr = 0xFF000000;
    svg.style.pft = pftNonZero;
    svg.AddPaths(subj_inner_all);
    svg.SaveToFile("solution_main_pre.svg");//, svg_scale);

    SVGBuilder svg_2;
    svg_2.style.brushClr = 0x3000FF00;
    svg_2.style.penClr = 0xFF006600;
    svg_2.style.pft = pftEvenOdd;
    svg_2.AddPaths(solution);
    svg_2.SaveToFile("solution_main_post.svg");//, svg_scale);

    //finally, show the svg image in the default viewing application (not working)
    // system("./solution_main.svg"); 
   
}


//////////////////////////////////////////////////////////////////////
////////////////////////////// Main //////////////////////////////////
//////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    // This is here because "true" and "false" are printed out as 1 and 0, whereas this fixes it.
    std::cout.setf(std::ios::boolalpha);


    std::vector<line> kings_crown;

    // point p11 = {10, 10};
    // point p12 = {10, 50};
    // point p13 = {20, 30};
    // point p14 = {30, 50};
    // point p15 = {40, 30};
    // point p16 = {50, 50};
    // point p17 = {60, 30};
    // point p18 = {70, 50};
    // point p19 = {80, 30};
    // point p20 = {90, 50};
    // point p21 = {100, 10};
    // point p22 = {110, 50};
    // point p23 = {120, 10};

    // line l1 = {p11, p12};
    // line l2 = {p12, p13};
    // line l3 = {p13, p14};
    // line l4 = {p14, p15};
    // line l5 = {p15, p16};
    // line l6 = {p16, p17};
    // line l7 = {p17, p18};
    // line l8 = {p18, p19};
    // line l9 = {p19, p20};
    // line l10 = {p20, p21};
    // line l11 = {p21, p11};

    // kings_crown.push_back(l1);
    // kings_crown.push_back(l2);
    // kings_crown.push_back(l3);
    // kings_crown.push_back(l4);
    // kings_crown.push_back(l5);
    // kings_crown.push_back(l6);
    // kings_crown.push_back(l7);
    // kings_crown.push_back(l8);
    // kings_crown.push_back(l9);
    // kings_crown.push_back(l10);
    // kings_crown.push_back(l11);




    std::vector<line> square;

    point p1 = {20, 20};
    point p2 = {20, 40};
    point p3 = {40, 40};
    point p4 = {40, 20};
    line l1 = {p1, p2};
    line l2 = {p2, p3};
    line l3 = {p3, p4};
    line l4 = {p4, p1};
    square.push_back(l1);
    square.push_back(l2);
    square.push_back(l3);
    square.push_back(l4);


    std::vector<line> square_2;
    p1 = {25, 25};
    p2 = {25, 35};
    p3 = {35, 35};
    p4 = {35, 25};
    l1 = {p1, p2};
    l2 = {p2, p3};
    l3 = {p3, p4};
    l4 = {p4, p1};
    square_2.push_back(l1);
    square_2.push_back(l2);
    square_2.push_back(l3);
    square_2.push_back(l4);


    std::vector<std::vector<std::vector<line>>> total_polys;
    std::vector<line> polygon = square;

    std::vector<std::vector<line>> poly_set;
    poly_set.push_back(polygon);

    
    total_polys.push_back(poly_set);

    poly_set.clear();
    poly_set.push_back(square_2);
    total_polys.push_back(poly_set);

    offset_total_polys(total_polys);


    // std::vector<line> new_polygon = offset_polygon(polygon, EXPAND);
    // std::cout << "Old polygon size: " << polygon.size() << std::endl;
    // std::cout << "New polygon size: " << new_polygon.size() << std::endl;
    // // new_polygon = reorder_cuts(new_polygon);
    // poly_set.push_back(new_polygon);

    // // poly_set.clear();
    // int xlims[2] = {0, 120};
    // int ylims[2] = {0, 120};
    // plot_poly_set(poly_set, xlims, ylims);

    return 0;

}