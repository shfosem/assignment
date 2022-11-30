#include "Simple_window.h"  // get access to our window library
#include "Graph.h"          // get access to our graphics library facilities
#include "std_lib_facilities.h"

using namespace Graph_lib;
int main()
{
	Point t1(100, 100);
	Simple_window win(t1, 800, 800, "shapes");
	
	Point t2(150, 150);
	Point t3(400, 150);
	Point t4(150, 400);
	Point t5(400, 400);

	Box b1(t2, 200, 200,50);
	b1.set_color(Color::black);
	b1.set_fill_color(Color::red);
	
	Box b2(t3, 100, 200, 10);
	b2.set_fill_color(Color::blue);

	Box b3(t4, 150, 200, 40);
	b3.set_color(Color::dark_blue);
	b3.set_fill_color(Color::green);

	Box b4(t5, 250, 100, 20);
	b4.set_fill_color(Color::white);


	win.attach(b1);
	win.attach(b2);
	win.attach(b3);
	win.attach(b4);
	win.wait_for_button();
	win.detach(b1);
	win.detach(b2);
	win.detach(b3);
	win.detach(b4);

	Regular_polygon rp1(t2, 3, 100);
	Regular_polygon rp2(t3, 5, 100);
	Regular_polygon rp3(t4, 6, 90);
	Regular_polygon rp4(t5, 10, 80);
	
	win.attach(rp1);
	win.attach(rp2);
	win.attach(rp3);
	win.attach(rp4);
	win.wait_for_button();
	win.detach(rp1);
	win.detach(rp2);
	win.detach(rp3);
	win.detach(rp4);

	Star st1(t2, 5, 100,50);
	Star st2(t3, 10, 30,120);
	Star st3(t4, 6, 80, 30);
	Star st4(t5, 7, 90, 50);

	st1.set_color(Color::yellow);
	st2.set_color(Color::dark_magenta);
	st3.set_style(Line_style::dashdot);
	st3.set_color(Color::dark_yellow);
	st4.set_color(Color::dark_cyan);
	
	win.attach(st1);
	win.attach(st2);
	win.attach(st3); 
	win.attach(st4);
	
	win.wait_for_button();

	win.detach(st1);
	win.detach(st2);
	win.detach(st3);
	win.detach(st4);

	win.wait_for_button();
}

