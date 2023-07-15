//
// Hello, flex_demo! Небольшие программы для изучения FLTK.
//
// [program/widget] is based in part on the work of the Fast Light Tool Kit (FLTK) project (https://www.fltk.org)
//

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <stdio.h>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Tree.H>

//==tree-simple.cxx

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
    const char* txt = ch->text();
    // See which style the user picked
    Fl::scheme(txt);
}

int main5(int argc, char* argv[]) {
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

int main(int argc, char** argv)
{
    int rc = 0;
    // задать стиль рисования контролов/виджетов
    Fl::scheme("gtk+");
    // запуск демо
    rc = main5(argc, argv);
    return rc;
}
