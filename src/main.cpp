#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>
#include "cStarterGUI.h"
#include "GraphTheory.h"

class cColor
{
public:
    void generate1();
    void generate2();
    void color();
    void display();

private:
    raven::graph::cGraph g;
    std::vector<int> vColor;

    std::string viz();
};

void cColor::generate1()
{
    g.clear();
    g.add("1", "2");
    g.add("2", "3");
    g.add("1", "3");
}
void cColor::generate2()
{
    g.clear();
    g.add("1", "2");
    g.add("1", "4");
    g.add("1", "6");
    g.add("3", "2");
    g.add("3", "4");
    g.add("3", "6");
    g.add("2", "5");
    g.add("4", "5");
    g.add("7", "8");
    g.add("7", "10");
    g.add("7", "12");
    g.add("9", "8");
    g.add("9", "10");
    g.add("9", "12");
    g.add("8", "11");
    g.add("10", "11");
    g.add("13", "14");
    g.add("13", "16");
    g.add("13", "18");
    g.add("15", "14");
    g.add("15", "16");
    g.add("15", "18");
    g.add("14", "17");
    g.add("16", "17");
    g.add("5", "11");
    g.add("12", "17");
    g.add("18", "6");
}

void cColor::color()
{
    vColor.clear();
    vColor.resize(2 * g.edgeCount(), 1);

    while (true) // loop A
    {
        int max = 0;
        int maxEdge = 0;

        for (int E = 0; E < 2 * g.edgeCount(); E++)
        {
            if (vColor[E] != 1)
                continue;
            if( g.src(E) > g.dest(E))
                continue;

            int S = 0;

            int vsrc = g.src(E);
            auto va = g.adjacentOut(vsrc);
            if (va.size() == 1)
                continue;
            for (int v : va)
            {
                if( vsrc > v )
                    continue;
                int ei = g.find(vsrc, v);
                if (vColor[ei] != 1)
                    continue;
                S++;
            }
            int vdst = g.dest(E);
            va = g.adjacentOut(vdst);
            if (va.size() == 1)
                continue;
            for (int v : va)
            {
                if( vdst > v )
                    continue;
                int ei = g.find(vdst,v);
                if (vColor[ei] != 1)
                    continue;
                S++;
            }

            //std::cout << g.userName(g.src(E)) <<" "<< g.userName(g.dest(E)) <<" "<< S << "\n";

            if (S > max)
            {
                max = S;
                maxEdge = E;
            }
        } // end loop E

        if (max == 0)
        {

            // finished

            int se = 0;
            for (auto e : g.edgeList())
            {
                int ei = g.find(e.first, e.second);
                se += vColor[ei];

                std::cout
                    << g.userName(g.src(ei))
                    << " " << g.userName(g.dest(ei))
                    << " color " << vColor[ei]
                    << "\n";
            }
            std::cout << "sum of all edge colors " << se << "\n";
            return;
        }

        for (int v : g.adjacentOut(g.src(maxEdge))) {
            int ie = g.find(g.src(maxEdge), v);
            if( vColor[ie] != 1 )
                continue;
            vColor[ie] = 0;
            //std::cout << g.userName(g.src(maxEdge)) <<" "<< g.userName(v) << " 0, ";
        }
        for (int v : g.adjacentOut(g.dest(maxEdge))) {
            int ie = g.find(g.dest(maxEdge), v);
            if( vColor[ie] != 1 )
                continue;
            vColor[ie] = 0;
            //std::cout << g.userName(g.dest(maxEdge)) <<" "<< g.userName(v) << " 0, ";
        }
        vColor[maxEdge] = 2;

        //std::cout << g.userName(g.src(maxEdge)) <<" "<< g.userName(g.dest(maxEdge)) << " 2\n";

    } // end loop A
}

void cColor::display()
{
    raven::graph::sGraphData gd;
    gd.g = g;
    RunDOT(
        gd.g,
        viz());
}

std::string cColor::viz()
{
    std::string graphvizgraph = "graph";
    std::string graphvizlink = "--";

    std::stringstream f;
    f << graphvizgraph << " G {\n";

    // loop over vertices
    for (int vi = 0; vi < g.vertexCount(); vi++)
    {
        f << g.userName(vi)
          << " [  penwidth = 3.0 ];\n";
    }

    // std::cout << "pathViz " << pathText() << "\n";

    // loop over edges
    for (int v1 = 0; v1 < g.vertexCount(); v1++)
    {
        for (int v2 : g.adjacentOut(v1))
        {
            // check not reverse link in undirected graph
            if (!g.isDirected())
                if (v1 > v2)
                    continue;

            f << g.userName(v1) << graphvizlink
              << g.userName(v2);

            // edge color
            static std::vector<std::string> vscol {  "black", "blue", "green", "red" };
            int ei = g.find(v1,v2);
            int icol = 3;
            // if( 0 <= ei && ei <= 3 )
                icol = vColor[ei];
            f << "[color=\"" << vscol[icol] << "\"] ";

            f << ";\n";
        }
    }
    f << "}\n";
    return f.str();
}

class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500})
    {
        color.generate1();
        color.color();

        // std::cout << "==================================\n";

        // color.generate2();
        // color.color();

         color.display();

        show();
        run();
    }

private:
    cColor color;
};

main()
{
    cGUI theGUI;
    return 0;
}
