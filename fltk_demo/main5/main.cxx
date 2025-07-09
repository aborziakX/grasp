// Hello, image_demo! Небольшие программы для изучения FLTK.
// [program/widget] is based in part on the work of the Fast Light Tool Kit (FLTK) project (https://www.fltk.org)

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <string>
#include <filesystem>  // filesystem library
#include <iostream>    // std::cout, std::endl
#include <stdio.h>      /* for printf and stderr */
#include <string.h>     /* for strerror */
#include <C:\Program Files\PostgreSQL\10\include\server\port\win32_msvc\dirent.h>     /* for DIR, opendir, readdir, and dirent */
#include <errno.h>      /* for errno */
#include <sys/stat.h>   /* for stat */

int main5(int argc, char** argv) 
{
    //test
    /*namespace fs = std::filesystem;
    const fs::path target_path{ "C:\\fltk-1.4\\FL" };

    try {
        for (auto const& dir_entry : fs::directory_iterator{ target_path })
        {
            if (fs::is_regular_file(dir_entry.path()))
            {
                std::cout << dir_entry.path().filename().string() << std::endl;
            }
        }
    }
    catch (fs::filesystem_error const& ex)
    {
        std::cout << "Error occurred during file operations!\n" << ex.what() << std::endl;
        return 1;
    }*/

    DIR* FD;        /* represent the directory */
    struct dirent* in_file;   /* represent the file */
    char* target_dir = "."; /* current directory */

    /* Scanning the target directory */
    FD = opendir(target_dir);
    if (FD == NULL)
    {
        fprintf(stderr, "Error: Failed to open input directory - %s\n", strerror(errno));
        return 1;
    }

    /* Reading object (files, directories ...) from the folder */
    while ((in_file = readdir(FD)))
    {
        struct stat buffer;
        int         status;

        status = stat(in_file->d_name, &buffer);
        /* check status */
        if (status == -1)
        {
            fprintf(stderr, "Error: Failed to stat item - %s\n", strerror(errno));
            return 1;
        }
        /* check result */
        if (buffer.st_mode & S_IFREG)
        {
            printf("%s is file \n", in_file->d_name);
        }
    }

    /* Close the directory */
    closedir(FD);

    return 0;


    // создать главное окно
    Fl_Window* window = new Fl_Window(400, 400);       
    // создать внутри рамку с текстом
    Fl_Box* box = new Fl_Box(20, 20, 350, 350, "Hello, flex_demo!");
    // загрузить файл в объект типа Fl_Shared_Image
    Fl_Shared_Image* img = Fl_Shared_Image::get("C:/grasp/fltk_demo/main5/tiny.bmp");
    // добавить картинку в рамку
    box->image(img);
    // масштабировоть картинку в рамке
    if( img ) img->scale(box->w(), box->h());
    // закончить добавление объектов
    window->end();
    // показать окно
    window->show(argc, argv);
    // выполнять до закрытия окна
    Fl::run();
    // освободить ресурсы
    if (img) 
    {
        ((Fl_Shared_Image*)box->image())->release();
        img = 0L;
    }
    return 0;
}

// главная функция
int main(int argc, char** argv)
{
    int rc = 0;
    // задать стиль рисования контролов/виджетов
    Fl::scheme("gtk+");
    // добавить поддержку изображений
    fl_register_images();
    // запуск демо
    rc = main5(argc, argv);
    return rc;
}
