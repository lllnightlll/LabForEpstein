// LabForEpsteinI.cpp : Этот файл содержит функцию "main". Здесь начинается и
// заканчивается выполнение программы.
//

#include <cstddef>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <list>
#include <string>


class IComponent {
public:
  virtual void add(IComponent *p) {}
  virtual void draw() = 0;
};

class Composite : public IComponent {
  std::list<IComponent *> li;

  virtual void add(IComponent *p) { li.push_back(p); }

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
  virtual void draw() { std::cout << "hello"; }
};

///// General Figure /////
class Figure {
public:
  std::list<Figure *> li;
  double relX = 0.5, relY = 0.5, relSize = 0.5;
  int widthLine = 3;
  std::string colour = "green";

  void setColour(std::string x) { colour = x; }

  virtual void add(Figure *p) { li.push_back(p); }

  virtual void draw(double cx, double cy, double size) {
    double ssize = size * relSize;
    double ccx = cx + (relX)*size;
    double ccy = cy + (relY - 0.5) * size;

    drawSelf(ccx, ccy, ssize);

    if (!li.empty()) {
      int n = li.size();

      int i = 0;
      for (auto *ch : li) {
        ch->relX = 0.5 + (i - n / 2.0) / n;
        ch->relY = 0.5;

        if (n > 2)
          ch->draw(ccx, ccy, ssize * 0.5);
        else
          ch->draw(ccx, ccy, ssize);
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
    std::cout << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"" << r
              << "\" fill=\"" << colour << "\" stroke-width=\"" << widthLine
              << "\" stroke=\"rgb(0,0,0)\" />\n";
  }
};

////// ====== //////
class Rect : public Figure {
  void drawSelf(double cx, double cy, double size) {
    double h = size / 2;
    std::cout << "<rect x=\"" << cx - h << "\" y=\"" << cy - h << "\" width=\""
              << size << "\" height=\"" << size << "\" fill=\"" << colour
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

    std::cout << "<polygon points=\"" << x1 << "," << y1 << " " << x2 << ","
              << y2 << " " << x3 << "," << y3 << "\" fill=\"" << colour
              << "\" stroke-width=\"" << widthLine
              << "\" stroke=\"rgb(0,0,0)\" />\n";
  }
};

///// General Math /////
class Math {
public:
  std::list<Math *> li;
  double relX = 0.5, relY = 0.5, relSize = 0.5;
  int widthLine = 3;

  virtual void add(Math *p) { li.push_back(p); }
  virtual void fillOperands() {}
  virtual void drawSelf(double cx, double cy, double size) = 0;
  virtual void draw(double cx, double cy, double size) {
    double ssize = size * relSize;
    double ccx = cx + (relX - 0.5) * size;
    double ccy = cy + (relY - 0.5) * size;

    fillOperands();
    drawSelf(ccx, ccy, ssize);

    for (auto *ch : li) {
      ch->draw(ccx, ccy, ssize * 0.7);
    }
  }
};

class Operand {
public:
  Math *x1;
  std::wstring x2;
  Operand(Math *x) : x1(x), x2() {}
  Operand(std::wstring x) : x1(nullptr), x2(x) {}
};

////// ====== //////
class Brackets : public Math {
public:
  void drawSelf(double cx, double cy, double size) {
    std::wcout << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\""
               << size * 0.6 << "\" fill=\"" << "black"
               << "\" stroke-width=\"" << 3
               << "\" stroke=\"rgb(0, 0, 0)\" />\n";

    std::wcout << L"<text\n"
               << L"xml:space=\"preserve\"\n"
               << L"style=\"font-size:" << size << L"px;letter-spacing:"
               << size * 5
               << L"px;writing-mode:lr-tb;direction:ltr;fill:#FFFFFF;\"\n"
               << L"x=\"0\"\n"
               << L"y=\"0\"\n"
               << L"transform=\"scale(0.1,1)\"\n"
               << L"id=\"text1\"><tspan\n"
               << L"sodipodi:role=\"line\"\n"
               << L"id=\"tspan1\"\n"
               << L"style=\"stroke-width:3\"\n"
               << L"stroke=\"red\"\n"
               << L"x=\"" << 10 * cx - 5.4 * size << L"\"\n"
               << L"y=\"" << cy + size * 0.225 << L"\">( )</tspan></text>";

    std::wcout
        << L"<text\n"
        << L"xml : space = \"preserve\"\n"
        << L"style=\"font-size:" << size / 9 << "px;letter-spacing:" << 0
        << L"px; writing - mode:lr - tb; direction:ltr; white - space:pre; "
           L"inline - size:0; fill:#FFFFFF; stroke - width:0.\"\n"
        << L"x=\"0\"\n"
        << L"y=\"0\"\n"
        << L"id=\"text2\"><tspan\n"
        << L"x=\"" << cx - size * 0.33 << "\"\n"
        << L"y=\"" << cy - size * 0.33 << "\"\n"
        << L"id=\"tspan7\">a₁₁&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;a₁₂&nbsp;&nbsp;&"
           L"nbsp;&nbsp;&nbsp;a₁₃\n"
        << L"</tspan><tspan\n"
        << L"x=\"" << cx - size * 0.33 << "\"\n"
        << L"y=\"" << cy << "\"\n"
        << L"id=\"tspan8\">a₂₁&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;a₂₂&nbsp;&nbsp;&"
           L"nbsp;&nbsp;&nbsp;a₂₃\n"
        << L"</tspan><tspan\n"
        << L"x=\"" << cx - size * 0.33 << "\"\n"
        << L"y=\"" << cy + size * 0.33 << "\"\n"
        << L"id=\"tspan9\">a₃₁&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;a₃₂&nbsp;&nbsp;&"
           L"nbsp;&nbsp;&nbsp;a₃₃</tspan></text>";
  }
};

////// ====== //////
class Integral : public Math {
public:
  Operand a = Operand(std::wstring(L"z"));

  void fillOperands() {
    auto it = li.begin();
    if (it != li.end()) {
      a = Operand(*it);
      li.erase(it);
    }
  }

  void drawSelf(double cx, double cy, double size) {
    std::wcout << "<circle cx=\"" << cx + size * 0.05 << "\" cy=\"" << cy
               << "\" r=\"" << size * 0.2 << "\" fill=\"" << "black"
               << "\" stroke-width=\"" << 3
               << "\" stroke=\"rgb(0, 0, 0)\" />\n";

    std::wcout << L"<text\n"
               << L"xml:space=\"preserve\"\n"
               << L"style=\"font-size:" << size / 5 << L"px;letter-spacing:"
               << size / 14
               << L"px;writing-mode:lr-tb;direction:ltr;fill:#FFFFFF;\"\n"
               << L"x=\"0\"\n"
               << L"y=\"0\"\n"
               << L"id=\"text1\"><tspan\n"
               << L"sodipodi:role=\"line\"\n"
               << L"id=\"tspan1\"\n"
               << L"x=\"" << cx - size * 0.15 << L"\"\n"
               << L"y=\"" << cy + size * 0.05 << L"\">∫ ʣ</tspan></text>";

    if (a.x1) {
      if (dynamic_cast<Brackets *>(a.x1) != nullptr)
        a.x1->draw(cx, cy, size / 3);
      else
        a.x1->draw(cx, cy, size);
    } else {
        std::wcout << L"<text\n"
            << L"xml : space = \"preserve\"\n"
            << L"style=\"font-size:" << size / 9 << "px;letter-spacing:" << 0 << L"px; writing - mode:lr - tb; direction:ltr; white - space:pre; inline - size:0; fill:#FFFFFF; stroke - width:0.\"\n"
            << L"x=\"0\"\n"
            << L"y=\"0\"\n"
            << L"id=\"text2\"><tspan\n"
            << L"x=\"" << cx << "\"\n"
            << L"y=\"" << cy << "\"\n"
            << L"id=\"tspan7\">" << a.x2 << "</tspan></text>";
        }
  }
};

////// ====== //////
class Devide : public Math {
public:
  Operand num = Operand(std::wstring(L"a")), den = Operand(std::wstring(L"b"));

  void fillOperands() {
    auto it = li.begin();
    if (it != li.end())
      num = Operand(*it);
    li.erase(it);
    it = li.begin();
    if (it != li.end())
      den = Operand(*it);
    li.erase(it);
  }

  void drawSelf(double cx, double cy, double size) {
    std::wcout << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\""
               << size * 0.2 << "\" fill=\"" << "black"
               << "\" stroke-width=\"" << 3
               << "\" stroke=\"rgb(0, 0, 0)\" />\n";

    std::wcout << L"<text\n"
               << L"xml:space=\"preserve\"\n"
               << L"style=\"font-size:20px;letter-spacing:0px;writing-mode:lr-"
                  L"tb;direction:ltr;fill:#FFFFFF;\"\n"
               << L"x=\"0\"\n"
               << L"y=\"0\"\n"
               << L"id=\"text1\"><tspan\n"
               << L"sodipodi:role=\"line\"\n"
               << L"id=\"tspan1\"\n"
               << L"x=\"" << cx - size * 0.25 << L"\"\n"
               << L"y=\"" << cy + size * 0.0025 << L"\"\n"
               << L"textLength=\"" << size * 0.5 << L"\"\n"
               << L"lengthAdjust=\"spacingAndGlyphs\">-</tspan></text>";

    if (num.x1) {
      if (dynamic_cast<Brackets *>(num.x1) != nullptr)
        num.x1->draw(cx, cy - size * 0.125, size / 3);
      else
        num.x1->draw(cx - size * 0.05, cy - size * 0.1, size * 0.9);
    } else {
        std::wcout << L"<text\n"
            << L"xml : space = \"preserve\"\n"
            << L"style=\"font-size:" << size / 9 << "px;letter-spacing:" << 0 << L"px; writing - mode:lr - tb; direction:ltr; white - space:pre; inline - size:0; fill:#FFFFFF; stroke - width:0.\"\n"
            << L"x=\"0\"\n"
            << L"y=\"0\"\n"
            << L"id=\"text2\"><tspan\n"
            << L"x=\"" << cx - size * 0.05 << "\"\n"
            << L"y=\"" << cy - size * 0.02 << "\"\n"
            << L"id=\"tspan7\">" << num.x2 << "</tspan></text>";
        }

    if (den.x1) {
      if (dynamic_cast<Brackets *>(den.x1) != nullptr)
        den.x1->draw(cx, cy + size * 0.1, size / 3);
      else
        den.x1->draw(cx - size * 0.05, cy + size * 0.09, size * 0.9);
    } else {
        std::wcout << L"<text\n"
            << L"xml : space = \"preserve\"\n"
            << L"style=\"font-size:" << size / 9 << "px;letter-spacing:" << 0 << L"px; writing - mode:lr - tb; direction:ltr; white - space:pre; inline - size:0; fill:#FFFFFF; stroke - width:0.\"\n"
            << L"x=\"0\"\n"
            << L"y=\"0\"\n"
            << L"id=\"text2\"><tspan\n"
            << L"x=\"" << cx - size * 0.05 << "\"\n"
            << L"y=\"" << cy + size * 0.075 << "\"\n"
            << L"id=\"tspan7\">" << den.x2 << "</tspan></text>";
        }
  }
};

/*void Math::draw(double cx, double cy, double size) {
        double ssize = size * relSize;
        double ccx = cx + (relX - 0.5)*size;
        double ccy = cy + (relY - 0.5) * size;

        drawSelf(ccx, ccy, ssize);

        /*if (!li.empty()) {
            int i = 0;
            for (auto* ch : li) {
                ch->relX = 0.5;
                ch->relY = 0.5;

                if(dynamic_cast<Brackets*>(ch) != nullptr) ch->draw(ccx, ccy,
   ssize / 3); else ch->draw(ccx, ccy, ssize); i++;
            }
        }*/

/*for (auto* ch : li) {
    ch->relX = 0.5;
    ch->relY = 0.5;

    if (auto* d = dynamic_cast<Devide*>(ch)) {
        auto it = d->li.begin();
        if (it != d->li.end()) {
            d->a = Operand(*it);
            ++it;
        }
        if (it != d->li.end()) {
            d->b = Operand(*it);
            ++it;
        }
        d->li.clear();
    }

    if (auto* d = dynamic_cast<Integral*>(ch)) {
        auto it = d->li.begin();
        if (it != d->li.end()) {
            d->a = Operand(*it);
            ++it;
        }
        d->li.clear();
    }

    ch->draw(ccx, ccy, ssize);  // рекурсивный обход вниз по дереву
}
}*/

int main() {
  _setmode(_fileno(stdout), _O_U16TEXT);
  /*
  IComponent* m[7] = { new Composite, new Composite,new Composite,new
  Composite,new Leaf,new Leaf,new Leaf}; m[0]->add(m[1]); m[0]->add(m[2]);
  m[1]->add(m[3]);
  m[1]->add(m[6]);
  m[4]->add(m[4]);
  m[4]->add(m[5]);

  m[0]->draw();*/

  /*
  Figure* m[9] = {new Circle, new Circle, new Rect, new Rect, new Rect, new
  Rect, new Circle, new Circle, new Poligon}; m[0]->add(m[2]); m[2]->add(m[1]);
  m[0]->add(m[3]);
  m[0]->add(m[4]);
  m[5]->add(m[0]);
  m[6]->add(m[5]);
  m[7]->add(m[6]);
  m[8]->add(m[7]);

  m[8]->draw(500, 500, 2000);*/

  Math *m[4] = {new Brackets, new Integral, new Devide, new Integral};
  // m[0]->add(m[1]);
  // m[1]->add(m[2]);

  int x = 700, y = 700;
  std::wcout << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << 10000
             << "\" fill=\"" << "black"
             << "\" stroke-width=\"" << 3 << "\" stroke=\"rgb(0,0,0)\" />\n";

  m[1]->add(m[2]);
  m[2]->add(m[3]);
  m[3]->add(m[0]);
  m[1]->draw(x, y, 3000);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и
//   другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый
//   элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий
//   элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" >
//   "Открыть" > "Проект" и выберите SLN-файл.
