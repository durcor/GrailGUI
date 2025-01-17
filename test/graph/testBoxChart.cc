#include "opengl/BoxChartWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/GraphStyle.hh"

using namespace std;
using namespace grail;

class TestBoxChart : public GraphStyle {
 public:
  TestBoxChart(Tab* tab) : GraphStyle(tab, "TIMES", 20, 12) {
    MainCanvas* c = tab->getMainCanvas();

    vector<double> data = {150, 350, 222, 100, 300,  //
                           130, 300, 250, 190, 170,  //
                           100, 50,  20,  150, 200,  //
                           330, 200, 270, 180, 300,  //
                           49,  247, 325, 114, 89};

    vector<string> names = {"red", "orange", "yellow", "green", "blue"};

    BoxChartWidget bcw(c, 100, 300, 850, 200, GraphWidget::AxisType::TEXT,
                       GraphWidget::AxisType::LINEAR, this);

    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    bcw.setGraphTitle("Test Title");

    // create x axis (categories)
    bcw.setNames(names);

    // set releavant x axis parameters
    // if you try to set something not applicable to a text axis (as that's what
    // the x axis always will be), the compiler will yell at you
    bcw.xAxis->setTitle("Colors");

    // y axis stuff
    bcw.setData(data);
    bcw.yAxis->setTitle("y axis");
    // linear
    bcw.yAxis->setBounds(0, 500);
    bcw.yAxis->setTickInterval(75);

    // set y axis parameters

    // bar chart widget specific bits
    bcw.setBoxWidth(45);
    bcw.setPointsPerBox(5);
    // bcw.setBoxColors(boxColors);
    // bcw.setWhiskerColors(whiskerColors);
    // bcw.setOutlineColors(outlineColors);

    bcw.init();
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* tab) {
  w->setTitle("Test Bar Chart");
  tab->addMember(new TestBoxChart(tab));
}
