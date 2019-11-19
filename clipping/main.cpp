#include "clipper.hpp"  
#include "clipper_console_demo.cpp"
#include "helper_functions.h"

using namespace ClipperLib;


int main(int argc, char** argv)
{

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

    Path subj;
    Paths whatever;
    Paths solution;

    for(int i = 0; i < square.size(); i++)
        subj << IntPoint(square[i].p1.x, square[i].p1.y);


  ClipperOffset co;
  co.AddPath(subj, jtMiter, etClosedPolygon);
  co.Execute(solution, -10.0);

  whatever << subj;

    //let's see the result too ...
  SVGBuilder svg;
  svg.style.penWidth = 1.5;
  svg.style.brushClr = 0x1200009C;
  svg.style.penClr = 0xFF000000;
  svg.style.pft = pftNonZero;
  svg.AddPaths(whatever);
  // svg.style.brushClr = 0x129C0000;
  // svg.style.penClr = 0xCCFFA07A;
  // svg.style.pft = pftNonZero;
  // svg.AddPaths(clip);
  svg.style.brushClr = 0x3000FF00;
  svg.style.penClr = 0xFF006600;
  svg.style.pft = pftNonZero;
  svg.AddPaths(solution);
  svg.SaveToFile("solution_main.svg");//, svg_scale);

  //finally, show the svg image in the default viewing application
  system("./solution_main.svg"); 

  std::cout << "Shrinking.." << std::endl;

  return 0;

}