// LabForEpsteinI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <list>
#include <fcntl.h>
#include <io.h>

class IComponent {
public:
    virtual void add(IComponent* p) {}
    virtual void draw() = 0;
};

class Composite : public IComponent {
    std::list<IComponent*> li;
    
    virtual void add(IComponent* p) {
        li.push_back(p);
    }

    virtual void draw() {
        std::cout << "(";
        for (auto it = li.begin(); it != li.end(); it++) {
            (*it)->draw();
            std::cout << ",";
        }
        std::cout << ")";
    }
};

class Leaf : public IComponent {
    virtual void draw() {
        std::cout << "hello";
    }
};


///// General Figure /////
class Figure {
public:    
    std::list <Figure*> li;
    double relX = 0.5,
        relY = 0.5,
        relSize = 0.5;
    int  widthLine = 3;
    std::string colour = "green";

    void setColour(std::string x) {
        colour = x;
    }

    virtual void add(Figure* p) { li.push_back(p); }

    virtual void draw(double cx, double cy, double size) {
        double ssize = size * relSize;
        double ccx = cx + (relX) * size;
        double ccy = cy + (relY - 0.5) * size;

        drawSelf(ccx, ccy, ssize);

        if (!li.empty()) {
            int n = li.size();        

            int i = 0;
            for (auto* ch : li) {
                ch->relX = 0.5 + (i - n / 2.0) / n;
                ch->relY = 0.5;

                if (n>2) ch->draw(ccx, ccy, ssize * 0.5);
                else ch->draw(ccx, ccy, ssize);
                i++;
            }
        }
    }

    virtual void drawSelf(double cx, double cy, double size) = 0;
};


////// ====== //////
class Circle : public Figure {
public:
    void drawSelf(double cx, double cy, double size) {
        double r = size / 2;
        std::cout << "<circle cx=\"" << cx
            << "\" cy=\"" << cy
            << "\" r=\"" << r
            << "\" fill=\"" << colour
            << "\" stroke-width=\"" << widthLine
            << "\" stroke=\"rgb(0,0,0)\" />\n";
    }
};


////// ====== //////
class Rect : public Figure {
    void drawSelf(double cx, double cy, double size) {
        double h = size / 2;
        std::cout << "<rect x=\"" << cx - h
            << "\" y=\"" << cy - h
            << "\" width=\"" << size
            << "\" height=\"" << size
            << "\" fill=\"" << colour
            << "\" stroke-width=\"" << widthLine
            << "\" stroke=\"rgb(0,0,0)\" />\n";
    }
};


////// ====== //////
class Poligon : public Figure {
    void drawSelf(double cx, double cy, double size) override {
        double h = size / 2.0;

        double x1 = cx - h * 1.2;
        double y1 = cy + h * 0.5;

        double x2 = cx + h * 1.2;
        double y2 = cy + h * 0.5;

        double x3 = cx;
        double y3 = cy - h * 0.7 * 1.2;   

        std::cout << "<polygon points=\""
            << x1 << "," << y1 << " "
            << x2 << "," << y2 << " "
            << x3 << "," << y3
            << "\" fill=\"" << colour
            << "\" stroke-width=\"" << widthLine
            << "\" stroke=\"rgb(0,0,0)\" />\n";
    }
};





///// General Math /////
class Math {
public:
    std::list <Math*> li;
    double relX = 0.33,
        relY = 0.33,
        relSize = 0.33;
    int  widthLine = 3;
    
    virtual void add(Math* p) { li.push_back(p); }

    virtual void draw(double cx, double cy, double size) {
        double ssize = size * relSize;
        double ccx = cx + (relX)*size;
        double ccy = cy + (relY - 0.33) * size;

        drawSelf(ccx, ccy, ssize);

        if (!li.empty()) {
            int n = li.size();

            int i = 0;
            for (auto* ch : li) {
                ch->relX = 0.33 + (i - n / 2.0) / n;
                ch->relY = 0.33;

                if (n > 2) ch->draw(ccx, ccy, ssize * 0.33);
                else ch->draw(ccx, ccy, ssize);
                i++;
            }
        }
    }

    virtual void drawSelf(double cx, double cy, double size) = 0;
};

////// ====== //////
class Brackets : public Math {
public:
    void drawSelf(double cx, double cy, double size) {
        std::wcout << L"<text\n"
            << L"xml:space=\"preserve\"\n"
            << L"style=\"font-size:" << size << L"px;letter-spacing:" << size << L"px;writing-mode:lr-tb;direction:ltr;fill:#000000;\"\n"
            << L"x=\"0\"\n"
            << L"y=\"0\"\n"
            << L"transform=\"scale(0.3,1)\"\n"
            << L"id=\"text1\"><tspan\n"
            << L"sodipodi:role=\"line\"\n"
            << L"id=\"tspan1\"\n"
            << L"style=\"stroke-width:3\"\n"
            << L"stroke=\"red\"\n"
            << L"x=\"" << cx << L"\"\n"
            << L"y=\"" << cy << L"\">( )</tspan></text>";

        std::wcout << L"<text\n"
            << L"xml : space = \"preserve\"\n"
            << L"style=\"font-size:" << size / 10 << "px;letter-spacing:" << 0 << L"px; writing - mode:lr - tb; direction:ltr; white - space:pre; inline - size:0; fill:#000000; stroke - width:0.\"\n"
            << L"x=\"0\"\n"
            << L"y=\"0\"\n"
            << L"id=\"text2\"><tspan\n"
            << L"x=\"" << cx - size * 1.05 << "\"\n"
            << L"y=\"" << cy - size/2.2 << "\"\n"
            << L"id=\"tspan7\">a₁₁&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;a₁₂&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;a₁₃\n"
            << L"</tspan><tspan\n"
            << L"x=\"" << cx - size * 1.05 << "\"\n"
            << L"y=\"" << cy + size / 5 - size/2.2 << "\"\n"
            << L"id=\"tspan8\">a₂₁&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;a₂₂&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;a₂₃\n"
            << L"</tspan><tspan\n"
            << L"x=\"" << cx - size * 1.05 << "\"\n"
            << L"y=\"" << cy + size * 2 / 5 - size/2.2 << "\"\n"
            << L"id=\"tspan9\">a₃₁&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;a₃₂&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;a₃₃</tspan></text>";
    }
};

////// ====== //////
class Integral : public Math {
public:
    void drawSelf(double cx, double cy, double size) {
        std::wcout << L"<text\n"
            << L"xml:space=\"preserve\"\n"
            << L"style=\"font-size:" << size/5 << L"px;letter-spacing:" << size/15 << L"px;writing-mode:lr-tb;direction:ltr;fill:#000000;\"\n"
            << L"x=\"0\"\n"
            << L"y=\"0\"\n"
            << L"id=\"text1\"><tspan\n"
            << L"sodipodi:role=\"line\"\n"
            << L"id=\"tspan1\"\n"
            << L"x=\"" << cx << L"\"\n"
            << L"y=\"" << cy << L"\">∫ dx</tspan></text>";

    }
};

////// ====== //////
class Devide : public Math {
public:
    void drawSelf(double cx, double cy, double size) {
        std::wcout << L"<text\n"
            << L"xml:space=\"preserve\"\n"
            << L"style=\"font-size:20px;letter-spacing:0px;writing-mode:lr-tb;direction:ltr;fill:#000000;\"\n"
            << L"x=\"0\"\n"
            << L"y=\"0\"\n"
            << L"id=\"text1\"><tspan\n"
            << L"sodipodi:role=\"line\"\n"
            << L"id=\"tspan1\"\n"
            << L"x=\"" << cx + size/1.5 << L"\"\n"
            << L"y=\"" << cy - size/6 << L"\"\n"
            << L"textLength=\"" << size * 0.5 << L"\"\n"
            << L"lengthAdjust=\"spacingAndGlyphs\">-</tspan></text>";

    }
};



int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    /*
    IComponent* m[7] = { new Composite, new Composite,new Composite,new Composite,new Leaf,new Leaf,new Leaf};
    m[0]->add(m[1]);
    m[0]->add(m[2]);
    m[1]->add(m[3]);
    m[1]->add(m[6]);
    m[4]->add(m[4]);
    m[4]->add(m[5]);

    m[0]->draw();*/


    /*
    Figure* m[9] = {new Circle, new Circle, new Rect, new Rect, new Rect, new Rect, new Circle, new Circle, new Poligon};
    m[0]->add(m[2]);
    m[2]->add(m[1]);
    m[0]->add(m[3]);
    m[0]->add(m[4]);
    m[5]->add(m[0]);
    m[6]->add(m[5]);
    m[7]->add(m[6]);
    m[8]->add(m[7]);

    m[8]->draw(500, 500, 2000);*/


    Math* m[3] = { new Brackets, new Integral, new Devide};
    m[0]->add(m[1]);
    m[1]->add(m[2]);

    m[0]->draw(700, 700, 3000);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
