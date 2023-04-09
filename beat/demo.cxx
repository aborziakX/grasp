//
// Hello, Beat! program to study FLTK.
//
// [program/widget] is based in part on the work of the Fast Light Tool Kit (FLTK) project (https://www.fltk.org)
//

#ifdef _WINDOWS
//#include <../build/config.h>
#endif
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/Fl_Input.H>
#include "GeObWindow.h"

using namespace Grasp;

int demo = 6;

//===from hello.cxx
int main1(int argc, char **argv) {
  Fl_Window *window = new Fl_Window(340, 180);
  Fl_Box *box = new Fl_Box(20, 40, 300, 100, "Hello, Beat!");
  box->box(FL_UP_BOX);
  box->labelfont(FL_BOLD + FL_ITALIC);
  box->labelsize(36);
  box->labeltype(FL_SHADOW_LABEL);
  window->end();
  window->show(argc, argv);
  return Fl::run();
}
//==

//==howto-browser-with-icons.cxx
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Pixmap.H>

static const char* big[] = {                       // XPM
  "50 34 4 1",
  "  c #000000",
  "o c #ff9900",
  "@ c #ffffff",
  "# c None",
  "##################################################",
  "###      ##############################       ####",
  "### ooooo  ###########################  ooooo ####",
  "### oo  oo  #########################  oo  oo ####",
  "### oo   oo  #######################  oo   oo ####",
  "### oo    oo  #####################  oo    oo ####",
  "### oo     oo  ###################  oo     oo ####",
  "### oo      oo                     oo      oo ####",
  "### oo       oo  ooooooooooooooo  oo       oo ####",
  "### oo        ooooooooooooooooooooo        oo ####",
  "### oo     ooooooooooooooooooooooooooo    ooo ####",
  "#### oo   ooooooo ooooooooooooo ooooooo   oo #####",
  "####  oo oooooooo ooooooooooooo oooooooo oo  #####",
  "##### oo oooooooo ooooooooooooo oooooooo oo ######",
  "#####  o ooooooooooooooooooooooooooooooo o  ######",
  "###### ooooooooooooooooooooooooooooooooooo #######",
  "##### ooooooooo     ooooooooo     ooooooooo ######",
  "##### oooooooo  @@@  ooooooo  @@@  oooooooo ######",
  "##### oooooooo @@@@@ ooooooo @@@@@ oooooooo ######",
  "##### oooooooo @@@@@ ooooooo @@@@@ oooooooo ######",
  "##### oooooooo  @@@  ooooooo  @@@  oooooooo ######",
  "##### ooooooooo     ooooooooo     ooooooooo ######",
  "###### oooooooooooooo       oooooooooooooo #######",
  "###### oooooooo@@@@@@@     @@@@@@@oooooooo #######",
  "###### ooooooo@@@@@@@@@   @@@@@@@@@ooooooo #######",
  "####### ooooo@@@@@@@@@@@ @@@@@@@@@@@ooooo ########",
  "######### oo@@@@@@@@@@@@ @@@@@@@@@@@@oo ##########",
  "########## o@@@@@@ @@@@@ @@@@@ @@@@@@o ###########",
  "########### @@@@@@@     @     @@@@@@@ ############",
  "############  @@@@@@@@@@@@@@@@@@@@@  #############",
  "##############  @@@@@@@@@@@@@@@@@  ###############",
  "################    @@@@@@@@@    #################",
  "####################         #####################",
  "##################################################",
};

static const char* med[] = {                       // XPM
  "14 14 2 1",
  "# c #000000",
  "  c #ffffff",
  "##############",
  "##############",
  "##          ##",
  "##  ##  ##  ##",
  "##  ##  ##  ##",
  "##   ####   ##",
  "##    ##    ##",
  "##    ##    ##",
  "##   ####   ##",
  "##  ##  ##  ##",
  "##  ##  ##  ##",
  "##          ##",
  "##############",
  "##############",
};

static const char* sml[] = {                       // XPM
  "9 11 5 1",
  ".  c None",
  "@  c #000000",
  "+  c #808080",
  "r  c #802020",
  "#  c #ff8080",
  ".........",
  ".........",
  "@+.......",
  "@@@+.....",
  "@@r@@+...",
  "@@##r@@+.",
  "@@####r@@",
  "@@##r@@+.",
  "@@r@@+...",
  "@@@+.....",
  "@+.......",
};

// Create a custom browser
//
//    You don't *have* to derive a class just to control icons in a browser,
//    but in final apps it's something you'd do to keep the implementation clean.
//
//    All it really comes down to is calling browser->icon() to define icons
//    for the items you want.
//
class MyBrowser : public Fl_Browser 
{
    Fl_Image* big_icon;
    Fl_Image* med_icon;
    Fl_Image* sml_icon;

public:
    MyBrowser(int X, int Y, int W, int H, const char* L = 0) : Fl_Browser(X, Y, W, H, L) 
    {
        // Create icons (these could also be pngs, jpegs..)
        big_icon = new Fl_Pixmap(big);
        med_icon = new Fl_Pixmap(med);
        sml_icon = new Fl_Pixmap(sml);

        // Normal browser initialization stuff
        textfont(FL_COURIER);
        textsize(14);
        type(FL_MULTI_BROWSER);
        add("<h1>One</h1>");
        add("Two");
        add("Three");
        add("Four");
        add("Five");
        add("Six");
        add("Seven");
    }

    static void Choice_CB(Fl_Widget* w, void* d) 
    {
        MyBrowser* mb = (MyBrowser*)d;
        Fl_Choice* ch = (Fl_Choice*)w;

        // See which icon the user picked
        Fl_Image* i = 0;
        if (strcmp(ch->text(), "None") == 0) { i = 0; }
        else if (strcmp(ch->text(), "Small") == 0) { i = mb->sml_icon; }
        else if (strcmp(ch->text(), "Medium") == 0) { i = mb->med_icon; }
        else if (strcmp(ch->text(), "Large") == 0) { i = mb->big_icon; }

        // Change the icon of three browser items to what the user picked
        //    This is all you have to do to change a browser item's icon.
        //    The browser will automatically resize the items if need be.
        mb->icon(3, i);
        mb->icon(4, i);
        mb->icon(5, i);
    }
};

int main2(int argc, char** argv)
{
    Fl_Double_Window* w = new Fl_Double_Window(400, 300);

    // Create a browser
    MyBrowser* b = new MyBrowser(10, 40, w->w() - 20, w->h() - 50);

    // Create a chooser to let the user change the icons
    Fl_Choice* choice = new Fl_Choice(60, 10, 140, 25, "Icon:");
    choice->add("None");
    choice->add("Small");
    choice->add("Medium");
    choice->add("Large");
    choice->callback(MyBrowser::Choice_CB, (void*)b);
    choice->take_focus();
    choice->value(1); choice->do_callback();

    w->end();
    w->show();
    return(Fl::run());
}
//==

//===progress-simle.cxx
#ifdef WIN32
// WINDOWS
#include <windows.h>
#define usleep(v) Sleep(v/1000)
#else /*WIN32*/
// UNIX
#include <unistd.h>                            // usleep
#endif /*WIN32*/
#include <FL/Fl_Progress.H>
#include <FL/Fl_Button.H>

// Button callback
void butt_cb(Fl_Widget* butt, void* data) 
{
    // Deactivate the button
    butt->deactivate();                        // prevent button from being pressed again                   
    Fl::check();                               // give fltk some cpu to gray out button
    // Make the progress bar
    Fl_Window* w = (Fl_Window*)data;           // access parent window
    w->begin();                                // add progress bar to it..
    Fl_Progress* progress = new Fl_Progress(10, 50, 200, 30);
    progress->minimum(0);                      // set progress range to be 0.0 ~ 1.0
    progress->maximum(1);
    progress->color(0x88888800);               // background color
    progress->selection_color(0x4444ff00);     // progress bar color
    progress->labelcolor(FL_WHITE);            // percent text color
    w->end();                                  // end adding to window
    // Computation loop..
    for (int t = 1; t <= 500; t++) 
    {
        progress->value((float)(t / 500.0));              // update progress bar with 0.0 ~ 1.0 value
        char percent[10];
        sprintf(percent, "%d%%", int((t / 500.0) * 100.0));
        progress->label(percent);              // update progress bar's label
        Fl::check();                           // give fltk some cpu to update the screen
        usleep(1000);                          // 'your stuff' that's compute intensive
    }
    // Cleanup
    w->remove(progress);                       // remove progress bar from window
    delete(progress);                          // deallocate it
    butt->activate();                          // reactivate button
    w->redraw();                               // tell window to redraw now that progress removed
}
// Main
int main3(int argc, char** argv) 
{
    Fl_Window win(220, 90);
    Fl_Button butt(10, 10, 100, 25, "Press");
    butt.callback(butt_cb, &win);
    win.resizable(win);
    win.show();
    return(Fl::run());
}
//==

//==tabs-simple.cxx
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
//
// Simple tabs example
//      _____  _____
//   __/ Aaa \/ Bbb \______________________
//  |    _______                           |
//  |   |_______|                          |
//  |    _______                           |
//  |   |_______|                          |
//  |    _______                           |
//  |   |_______|                          |
//  |______________________________________|
//
int main4(int argc, char* argv[]) {
    Fl::scheme("gtk+");
    Fl_Window* win = new Fl_Window(500, 200, "Tabs Example");
    {
        // Create the tab widget
        Fl_Tabs* tabs = new Fl_Tabs(10, 10, 500 - 20, 200 - 20);
        {
            // ADD THE "Aaa" TAB
            //   We do this by adding a child group to the tab widget.
            //   The child group's label defined the label of the tab.
            //
            Fl_Group* aaa = new Fl_Group(10, 35, 500 - 20, 200 - 45, "Aaa");
            {
                // Put some different buttons into the group, which will be shown
                // when the tab is selected.
                Fl_Button* b1 = new Fl_Button(50, 60, 90, 25, "Button A1"); b1->color(88 + 1);
                Fl_Button* b2 = new Fl_Button(50, 90, 90, 25, "Button A2"); b2->color(88 + 2);
                Fl_Button* b3 = new Fl_Button(50, 120, 90, 25, "Button A3"); b3->color(88 + 3);
            }
            aaa->end();

            // ADD THE "Bbb" TAB
            //   Same details as above.
            //
            Fl_Group* bbb = new Fl_Group(10, 35, 500 - 10, 200 - 35, "Bbb");
            {
                // Put some different buttons into the group, which will be shown
                // when the tab is selected.
                Fl_Button* b1 = new Fl_Button(50, 60, 90, 25, "Button B1"); b1->color(88 + 1);
                Fl_Button* b2 = new Fl_Button(150, 60, 90, 25, "Button B2"); b2->color(88 + 3);
                Fl_Button* b3 = new Fl_Button(250, 60, 90, 25, "Button B3"); b3->color(88 + 5);
                Fl_Button* b4 = new Fl_Button(50, 90, 90, 25, "Button B4"); b4->color(88 + 2);
                Fl_Button* b5 = new Fl_Button(150, 90, 90, 25, "Button B5"); b5->color(88 + 4);
                Fl_Button* b6 = new Fl_Button(250, 90, 90, 25, "Button B6"); b6->color(88 + 6);
            }
            bbb->end();
        }
        tabs->end();
    }
    win->end();
    win->show(argc, argv);
    return(Fl::run());
}
//==

//==tree-simple.cxx
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Tree.H>

// Tree's callback
//    Invoked whenever an item's state changes.
//
void TreeCallback(Fl_Widget* w, void* data) {
    Fl_Tree* tree = (Fl_Tree*)w;
    Fl_Tree_Item* item = (Fl_Tree_Item*)tree->callback_item();
    if (!item) return;
    switch (tree->callback_reason()) {
    case FL_TREE_REASON_SELECTED: {
        char pathname[256];
        tree->item_pathname(pathname, sizeof(pathname), item);
        fprintf(stderr, "TreeCallback: Item selected='%s', Full pathname='%s'\n", item->label(), pathname);
        break;
    }
    case FL_TREE_REASON_DESELECTED:
        // fprintf(stderr, "TreeCallback: Item '%s' deselected\n", item->label());
        break;
    case FL_TREE_REASON_OPENED:
        // fprintf(stderr, "TreeCallback: Item '%s' opened\n", item->label());
        break;
    case FL_TREE_REASON_CLOSED:
        // fprintf(stderr, "TreeCallback: Item '%s' closed\n", item->label());
        break;
#if FLTK_ABI_VERSION >= 10301
        // To enable this callback, use tree->item_reselect_mode(FL_TREE_SELECTABLE_ALWAYS);
    case FL_TREE_REASON_RESELECTED:
        // fprintf(stderr, "TreeCallback: Item '%s' reselected\n", item->label());
        break;
#endif
    default:
        break;
    }
}

static void Choice_CB5(Fl_Widget* w, void* d)
{
    Fl_Double_Window* mb = (Fl_Double_Window*)d;
    Fl_Choice* ch = (Fl_Choice*)w;
    const char * txt = ch->text();
    // See which style the user picked
    Fl::scheme(txt);
}

int main5(int argc, char* argv[]) {
    //Fl::scheme("base");
    Fl_Double_Window* win = new Fl_Double_Window(300, 400, "Simple Tree");
    win->begin();
    {
        // Create the tree
        Fl_Tree* tree = new Fl_Tree(10, 40, win->w() - 20, win->h() - 50);
        tree->showroot(0);				// don't show root of tree
        tree->callback(TreeCallback);		// setup a callback for the tree

        // Add some items
        tree->add("Flintstones/Fred");
        tree->add("Flintstones/Wilma");
        tree->add("Flintstones/Pebbles");
        tree->add("Simpsons/Homer");
        tree->add("Simpsons/Marge");
        tree->add("Simpsons/Bart");
        tree->add("Simpsons/Lisa");
        tree->add("Pathnames/\\/bin");		// front slashes
        tree->add("Pathnames/\\/usr\\/sbin");
        tree->add("Pathnames/C:\\\\Program Files");	// backslashes
        tree->add("Pathnames/C:\\\\Documents and Settings");

        // Start with some items closed
        tree->close("Simpsons");
        tree->close("Pathnames");
    }

    // Create a chooser to let the user change the style
    Fl_Choice* choice = new Fl_Choice(40, 10, 140, 25, "Style:");
    choice->add("base"); //This is an alias for "none" = old Windows
    choice->add("gtk+"); //This scheme is inspired by the Red Hat Bluecurve theme
    choice->add("plastic"); //This scheme is inspired by the Aqua user interface on Mac OS X
    choice->add("gleam"); //This scheme is inspired by the Clearlooks Glossy scheme
    choice->add("oxy"); //This is a subset of Dmitrij K's oxy scheme (STR 2675, 3477)

    choice->callback(Choice_CB5, (void*)win);
    choice->take_focus();
    choice->value(1); choice->do_callback();

    Fl_Button butt(190, 10, 60, 25, "Press");

    win->end();
    win->resizable(win);
    //win->show(argc, argv);
    win->show();
    return (Fl::run());
}
//==

//==cube.cxx
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Radio_Light_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/fl_ask.H>
#include <stdlib.h>
#include <FL/Fl_Menu_Bar.H>

class cube_box : public Fl_Gl_Window {
  void draw() FL_OVERRIDE;
  int handle(int) FL_OVERRIDE;

public:
  double lasttime;
  int wire=0;
  double size=1;
  double speed=1;
  cube_box(int x, int y, int w, int h, const char *l = 0)
    : Fl_Gl_Window(x, y, w, h, l) 
  {
    lasttime = 0.0;
    box(FL_DOWN_FRAME);
  }
};

/* The cube definition */
float v0[3] = {0.0, 0.0, 0.0};
float v1[3] = {1.0, 0.0, 0.0};
float v2[3] = {1.0, 1.0, 0.0};
float v3[3] = {0.0, 1.0, 0.0};
float v4[3] = {0.0, 0.0, 1.0};
float v5[3] = {1.0, 0.0, 1.0};
float v6[3] = {1.0, 1.0, 1.0};
float v7[3] = {0.0, 1.0, 1.0};

#define v3f(x) glVertex3fv(x)

void drawcube(int wire) 
{
  /* Draw a colored cube */
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0, 0, 255);
  v3f(v0);
  v3f(v1);
  v3f(v2);
  v3f(v3);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0, 255, 255);
  v3f(v4);
  v3f(v5);
  v3f(v6);
  v3f(v7);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255, 0, 255);
  v3f(v0);
  v3f(v1);
  v3f(v5);
  v3f(v4);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255, 255, 0);
  v3f(v2);
  v3f(v3);
  v3f(v7);
  v3f(v6);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(0, 255, 0);
  v3f(v0);
  v3f(v4);
  v3f(v7);
  v3f(v3);
  glEnd();
  glBegin(wire ? GL_LINE_LOOP : GL_POLYGON);
  glColor3ub(255, 0, 0);
  v3f(v1);
  v3f(v2);
  v3f(v6);
  v3f(v5);
  glEnd();
}

void cube_box::draw() 
{
  lasttime = lasttime + speed;
  if (!valid()) {
    glLoadIdentity();
    glViewport(0, 0, pixel_w(), pixel_h());
    glEnable(GL_DEPTH_TEST);
    glFrustum(-1, 1, -1, 1, 2, 10000);
    glTranslatef(0, 0, -10);
    glClearColor(0.4f, 0.4f, 0.4f, 0);
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glRotatef(float(lasttime * 1.6), 0, 0, 1);
  glRotatef(float(lasttime * 4.2), 1, 0, 0);
  glRotatef(float(lasttime * 2.3), 0, 1, 0);
  glTranslatef(-1.0f, 1.2f, -1.5f);
  glScalef(float(size), float(size), float(size));
  drawcube(wire);
  glPopMatrix();
  gl_color(FL_GRAY);
  glDisable(GL_DEPTH_TEST);
  gl_font(FL_HELVETICA_BOLD, 16);
  gl_draw(wire ? "Cube: wire" : "Cube: flat", -4.5f, -4.5f);
  glEnable(GL_DEPTH_TEST);

  // if an OpenGL graphics driver is installed, give it a chance
  // to draw additional graphics
  Fl_Gl_Window::draw();
}

int cube_box::handle(int e) 
{
  switch (e) {
    case FL_ENTER:
      cursor(FL_CURSOR_CROSS);
      break;
    case FL_LEAVE:
      cursor(FL_CURSOR_DEFAULT);
      break;
  }
  return Fl_Gl_Window::handle(e);
}

Fl_Window *form;
Fl_Slider *speed, *slSize;
Fl_Button *exit_button, *wire, *flat;
cube_box *lt_cube, *rt_cube;

int done = 0; // set to 1 in exit button callback

// exit button callback
void exit_cb(Fl_Widget *, void *) 
{
  done = 1;
}

// print screen demo
void print_cb(Fl_Widget *w, void *data) 
{
  fl_alert("Quantum fluctuations in the space-time continuum detected,\n"
             "you have %g seconds to comply.\n\n"
             "\"In physics, spacetime is any mathematical model that combines\n"
             "space and time into a single construct called the space-time\n"
             "continuum. Spacetime is usually interpreted with space being\n"
             "three-dimensional and time playing the role of the\n"
             "fourth dimension.\" - Wikipedia",
             10.0);
  }
  // end of printing demo

// Create a form that allows resizing for A and C (GL windows) with B fixed size/centered:
//
//                  lt_grp                                rt_grp
//      |<--------------------------------------->|<---------------------->|
//      .          lt_cube            ct_grp      :       rt_cube          .
//      .            350                100       :         350            .
//      .  |<------------------->|  |<-------->|  |<------------------->|  .
//      ....................................................................
//      :  .......................  ............  .......................  :
//      :  :                     :  :          :  :                     :  :
//      :  :          A          :  :    B     :  :          C          :  :
//      :  :                     :  :          :  :                     :  :
//      :  :.....................:  :..........:  :.....................:  :  __
//      :..................................................................:  __ MARGIN
//
//      |  |
//     MARGIN
//

#define MENUBAR_H 25 // menubar height
#define MARGIN 20    // fixed margin around widgets
#define MARGIN2 (MARGIN * 2)
#define MARGIN3 (MARGIN * 3)

void show_info_cb(Fl_Widget *, void *) 
{
    //fl_close = "Закрыть";
  fl_message("This is an example of using FLTK widgets inside OpenGL windows.\n"
             "Multiple widgets can be added to Fl_Gl_Windows. They will be\n"
             "rendered as overlays over the scene.");
}

void makeform6(const char *name) 
{
  // Widget's XYWH's
  int form_w = 800 + 4 * MARGIN;                                           // main window width
  int form_h = 350 + MENUBAR_H + 2 * MARGIN;                               // main window height
  int me_bar_x = 0, me_bar_y = 0, me_bar_w = form_w, me_bar_h = MENUBAR_H; // menubar
  int lt_grp_x = 0, lt_grp_y = MENUBAR_H + MARGIN, lt_grp_w = 350 + 100 + MARGIN3,
      lt_grp_h = form_h - MENUBAR_H - MARGIN2; // left group
  int lt_cub_x = lt_grp_x + MARGIN, lt_cub_y = lt_grp_y, lt_cub_w = 350,
      lt_cub_h = lt_grp_h; // left cube box (GL)
  int ct_grp_x = lt_grp_x + 350 + MARGIN2, ct_grp_y = lt_grp_y, ct_grp_w = 100,
      ct_grp_h = lt_grp_h; // center group
  int rt_grp_x = lt_grp_x + lt_grp_w, rt_grp_y = lt_grp_y, rt_grp_w = 350 + MARGIN,
      rt_grp_h = lt_grp_h; // right group
  int rt_cub_x = rt_grp_x, rt_cub_y = lt_grp_y, rt_cub_w = 350,
      rt_cub_h = lt_grp_h; // right cube box (GL)

  // main window
  form = new Fl_Window(form_w, form_h, name);
  form->begin();
  // menu bar
  Fl_Menu_Bar *menubar = new Fl_Menu_Bar(me_bar_x, me_bar_y, me_bar_w, me_bar_h);
  menubar->add("File/Print window", FL_COMMAND + 'p', print_cb);
  menubar->add("File/Quit", FL_COMMAND + 'q', exit_cb);
  // left group
  Fl_Group *lt_grp = new Fl_Group(lt_grp_x, lt_grp_y, lt_grp_w, lt_grp_h);
  lt_grp->begin();
  // left GL window
  lt_cube = new cube_box(lt_cub_x, lt_cub_y, lt_cub_w, lt_cub_h, 0);

  lt_cube->begin();
  Fl_Widget *w = new Fl_Button(10, 10, 120, 30, "FLTK over GL");
  w->color(FL_FREE_COLOR);
  w->box(FL_BORDER_BOX);
  w->callback(show_info_cb);
  lt_cube->end();

  // center group
  Fl_Group *ct_grp = new Fl_Group(ct_grp_x, ct_grp_y, ct_grp_w, ct_grp_h);
  ct_grp->begin();
  wire = new Fl_Radio_Light_Button(ct_grp_x, ct_grp_y, 100, 25, "Wire");
  flat = new Fl_Radio_Light_Button(ct_grp_x, wire->y() + wire->h(), 100, 25, "Flat");
  speed = new Fl_Slider(FL_VERT_SLIDER, ct_grp_x, flat->y() + flat->h() + MARGIN, 40, 200, "Speed");
  slSize = new Fl_Slider(FL_VERT_SLIDER, ct_grp_x + 40 + MARGIN, flat->y() + flat->h() + MARGIN, 40,
                       200, "Size");
  exit_button = new Fl_Button(ct_grp_x, form_h - MARGIN - 25, 100, 25, "Exit");
  exit_button->callback(exit_cb);
  ct_grp->end();
  ct_grp->resizable(speed); // only sliders resize vertically, not buttons
  lt_grp->end();
  lt_grp->resizable(lt_cube);
  // right group
  Fl_Group *rt_grp = new Fl_Group(rt_grp_x, rt_grp_y, rt_grp_w, rt_grp_h);
  rt_grp->begin();
  // right GL window
  rt_cube = new cube_box(rt_cub_x, rt_cub_y, rt_cub_w, rt_cub_h, 0);
  rt_grp->end();
  rt_grp->resizable(rt_cube);
  // right resizer
  Fl_Box *rt_resizer = new Fl_Box(rt_grp_x - 5, rt_grp_y, 10, rt_grp_h);
  rt_resizer->box(FL_NO_BOX);

  form->end();
  form->resizable(rt_resizer);
  form->size_range(form->w(), form->h()); // minimum window size
}

int main6(int argc, char **argv) {
  Fl::use_high_res_GL(1);
  Fl::set_color(FL_FREE_COLOR, 255, 255, 0, 75);
  makeform6(argv[0]);
  speed->bounds(4, 0);
  speed->value(lt_cube->speed = rt_cube->speed = 1.0);

  slSize->bounds(4, 0.01);
  slSize->value(lt_cube->size = rt_cube->size = 3.0);
  flat->value(1);
  lt_cube->wire = 0;
  rt_cube->wire = 1;
  form->label("cube");
  form->show();//argc, argv);
  lt_cube->show();
  rt_cube->show();
  for (;;) {
    if (form->visible() && speed->value()) {
      if (!Fl::check())
        break; // returns immediately
    } else {
      if (!Fl::wait())
        break; // waits until something happens
    }
    lt_cube->wire = wire->value();
    rt_cube->wire = !wire->value();
    lt_cube->size = rt_cube->size = slSize->value();
    lt_cube->speed = rt_cube->speed = speed->value() * 0.3;
    lt_cube->redraw();
    rt_cube->redraw();
    if (done)
      break; // exit button was clicked
  }
  return 0;
}
//==

int main(int argc, char** argv)
{
    int rc = 0;
    Fl::scheme("gtk+");
    if (argc > 1)
      demo = atoi(argv[1]);
    if (demo == 1) rc = main1(argc, argv);
    else if (demo == 2) rc = main2(argc, argv);
    else if (demo == 3) rc = main3(argc, argv);
    else if (demo == 4) rc = main4(argc, argv);
    else if (demo == 5) rc = main5(argc, argv);
    else if (demo == 6) rc = main6(argc, argv);
    return rc;
}


