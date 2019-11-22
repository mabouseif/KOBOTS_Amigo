#include "clipper.hpp"  
#include "clipper_console_demo.cpp"
#include "../helper_functions.h"
#include "../matplotlibcpp.h"

using namespace ClipperLib;

#define OFFSET_SHRINK 0
#define OFFSET_EXPAND 1

void offset_polygon(std::vector<line>& polygon, int mode)
{
    int offset_val;
    switch(mode)
    {
        case(0): 
            std::cout << "Shrinking polygon by offset" << std::endl;
            offset_val = -10;
            break;
        
        case(1):
            std::cout << "Expanding polygon by offset" << std::endl;
            offset_val = 10;
            break;

        default:
            std::cout << "Invalid offset mode!" << std::endl;
            exit (EXIT_FAILURE);
    }
    
    std::cout << "Shrinking.." << std::endl;

    // ClipperLib-related variables
    Path subj;
    Paths original;
    Paths solution;

    // Convert to ClipperLib format
    for(int i = 0; i < polygon.size(); i++)
        subj << IntPoint(polygon[i].p1.x, polygon[i].p1.y);

    // Offset using passed args
    ClipperOffset co;
    co.AddPath(subj, jtMiter, etClosedPolygon);
    co.Execute(solution, -10.0);

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
    int mod_num = (poly_or_not) ? n:INT32_MAX;
    
    polygon.clear();

    for(int i = 0; i < n; i++)
    {
        std::cout << "mod num" << (i+1)%mod_num << std::endl;
        temp_line.p1.x = solution[0][i].X;
        temp_line.p1.y = solution[0][i].Y;
        temp_line.p2.x = solution[0][(i+1)%mod_num].X;
        temp_line.p2.y = solution[0][(i+1)%mod_num].Y; // using the modulu operator
        polygon.push_back(temp_line);
    }

    // Visual content check
    for(int i = 0; i < solution[0].size(); i++)
    {
      std::cout << "(" << solution[0][i].X<< ", " << solution[0][i].Y << ")" << std::endl;
      std::cout << "(" << polygon[i].p1.x << ", " << polygon[i].p1.y << ")\n" << std::endl;
      std::cout << std::endl;
    }


    //let's see the result too ...
    original << subj;

    SVGBuilder svg;
    svg.style.penWidth = 1.5;
    svg.style.brushClr = 0x1200009C;
    svg.style.penClr = 0xFF000000;
    svg.style.pft = pftNonZero;
    svg.AddPaths(original);
    // svg.style.brushClr = 0x129C0000;
    // svg.style.penClr = 0xCCFFA07A;
    // svg.style.pft = pftNonZero;
    // svg.AddPaths(clip);
    svg.style.brushClr = 0x3000FF00;
    svg.style.penClr = 0xFF006600;
    svg.style.pft = pftNonZero;
    svg.AddPaths(solution);
    svg.SaveToFile("solution_main.svg");//, svg_scale);

    //finally, show the svg image in the default viewing application (not working)
    system("./solution_main.svg"); 

}

//////////////////////////////////////////////////////////////////////
////////////////////////////// Main //////////////////////////////////
//////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    // This is here because "true" and "false" are printed out as 1 and 0, whereas this fixes it.
    std::cout.setf(std::ios::boolalpha);


    std::vector<line> square;
    point p1 = {200, 200};
    point p2 = {200, 400};
    point p3 = {400, 400};
    point p4 = {400, 200};
    line l1 = {p1, p2};
    line l2 = {p2, p3};
    line l3 = {p3, p4};
    line l4 = {p4, p1};
    square.push_back(l1);
    square.push_back(l2);
    square.push_back(l3);
    square.push_back(l4);

    std::vector<std::vector<line>> poly_set;
    poly_set.push_back(square);

    int xlims[2] = {0, 500};
    int ylims[2] = {0, 500};
    plot_poly_set(poly_set, xlims, ylims);

    offset_shrink(square);

    poly_set.clear();
    poly_set.push_back(square);
    plot_poly_set(poly_set, xlims, ylims);

    return 0;

}