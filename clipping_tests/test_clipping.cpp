#include "clipper.hpp"  
#include "clipper_console_demo.cpp"

using namespace ClipperLib;
 

int main()
{
  Path subj;
  Paths whatever;
  Paths solution;
  subj << 
    // IntPoint(348,257) << IntPoint(364,148) << IntPoint(362,148) << 
    // IntPoint(326,241) << IntPoint(295,219) << IntPoint(258,88) << 
    // IntPoint(440,129) << IntPoint(370,196) << IntPoint(372,275);
    IntPoint(300,200) << IntPoint(300,400) << IntPoint(400,400) << 
    IntPoint(400,200) << IntPoint(300, 200);
    
  ClipperOffset co;
  co.AddPath(subj, jtMiter, etClosedPolygon);
  co.Execute(solution, -2.0);
   
  //draw solution ...
  // DrawPolygons(solution, 0x4000FF00, 0xFF009900);
  whatever << subj;

    //let's see the result too ...
  SVGBuilder svg;
  svg.style.penWidth = 0.8;
  svg.style.brushClr = 0x1200009C;
  svg.style.penClr = 0xFF000000;
  svg.style.pft = pftNonZero;
  svg.AddPaths(whatever);
  // svg.style.brushClr = 0x129C0000;
  // svg.style.penClr = 0xCCFFA07A;
  // svg.style.pft = pftNonZero;
  // svg.AddPaths(clip);
  svg.style.brushClr = 0xFF003300;
  svg.style.penClr = 0xFF003300;
  svg.style.pft = pftNonZero;
  svg.AddPaths(solution);
  svg.SaveToFile("solution.svg");//, svg_scale);

  //finally, show the svg image in the default viewing application
  system("./solution.svg"); 

  return 0;
}