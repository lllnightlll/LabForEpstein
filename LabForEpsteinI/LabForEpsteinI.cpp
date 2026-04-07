// LabForEpsteinI.cpp : Этот файл содержит функцию "main". Здесь начинается и
// заканчивается выполнение программы.
//

#include <cstddef>
#include <fcntl.h>
#include <fstream>
#include <io.h>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <windows.h>


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
  Operand a11 = Operand(std::wstring(L"a11")),
          a22 = Operand(std::wstring(L"a22")),
          a33 = Operand(std::wstring(L"a33"));

  void fillOperands() {
    auto it = li.begin();
    if (it != li.end()) {
      a11 = Operand(*it);
      li.erase(it);
    }
    it = li.begin();
    if (it != li.end()) {
      a22 = Operand(*it);
      li.erase(it);
    }
    it = li.begin();
    if (it != li.end()) {
      a33 = Operand(*it);
      li.erase(it);
    }
  }

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

    if (a11.x1) {
      if (dynamic_cast<Brackets *>(a11.x1) != nullptr)
        a11.x1->draw(cx - size * 0.27, cy - size * 0.35, size / 3);
      else
        a11.x1->draw(cx - size * 0.27, cy - size * 0.35, size);
    }
    if (a22.x1) {
      if (dynamic_cast<Brackets *>(a22.x1) != nullptr)
        a22.x1->draw(cx - size * 0.03, cy - size * 0.03, size / 3);
      else
        a22.x1->draw(cx - size * 0.03, cy - size * 0.03, size);
    }
    if (a33.x1) {
      if (dynamic_cast<Brackets *>(a33.x1) != nullptr)
        a33.x1->draw(cx + size * 0.25, cy + size * 0.3, size / 3);
      else
        a33.x1->draw(cx + size * 0.25, cy + size * 0.3, size);
    }
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
      std::wcout
          << L"<text\n"
          << L"xml : space = \"preserve\"\n"
          << L"style=\"font-size:" << size / 9 << "px;letter-spacing:" << 0
          << L"px; writing - mode:lr - tb; direction:ltr; white - space:pre; "
             L"inline - size:0; fill:#FFFFFF; stroke - width:0.\"\n"
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
    if (it != li.end()) {
      num = Operand(*it);
      li.erase(it);
    }
    it = li.begin();
    if (it != li.end()) {
      den = Operand(*it);
      li.erase(it);
    }
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
      std::wcout
          << L"<text\n"
          << L"xml : space = \"preserve\"\n"
          << L"style=\"font-size:" << size / 9 << "px;letter-spacing:" << 0
          << L"px; writing - mode:lr - tb; direction:ltr; white - space:pre; "
             L"inline - size:0; fill:#FFFFFF; stroke - width:0.\"\n"
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
      std::wcout
          << L"<text\n"
          << L"xml : space = \"preserve\"\n"
          << L"style=\"font-size:" << size / 9 << "px;letter-spacing:" << 0
          << L"px; writing - mode:lr - tb; direction:ltr; white - space:pre; "
             L"inline - size:0; fill:#FFFFFF; stroke - width:0.\"\n"
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

////// lab2 //////
class View {
public:
  virtual void update(std::list<double> data, std::fstream &file) = 0;
  virtual ~View() = default;
};

class view1 : public View {
public:
  void update(std::list<double> data, std::fstream &file) override {
    file << "<title>Expenses</title>\n<g id=\"rowGroup\" "
            "transform=\"translate(0, 0)\" role=\"table\">\n<rect x=\"25\" "
            "y=\"27\" width=\"3000\" height=\"20\" fill=\"gainsboro\"/>\n<text "
            "x=\"30\" y=\"20\" font-size=\"18px\" font-weight=\"bold\" "
            "fill=\"crimson\" text-anchor=\"middle\" role=\"row\">\n";
    for (unsigned int i = 0; i < data.size(); i++) {
      file << "<tspan role=\"columnheader\" x=\"" << (i + 1) * 100 << "\">"
           << (char)(i + 65) << "</tspan>\n";
    }
    file << "</text>\n<text x=\"30\" y=\"42\" font-size=\"18px\" "
            "font-weight=\"bold\" fill=\"crimson\" text-anchor=\"middle\" "
            "role=\"row\">\n";

    int i = 1;
    for (auto it = data.begin(); it != data.end(); ++it, ++i) {
      file << "<tspan role=\"columnheader\" x=\"" << i * 100 << "\">" << *it
           << "</tspan>\n";
    }
  }
};

class view2 : public View {
public:
  void update(std::list<double> data, std::fstream &file) override {
    file << "<title>Expenses</title> <!-- Ось Y с процентами (слева) -->\n<g "
            "font-size=\"12\" fill=\"#333\">\n<text x=\"30\" "
            "y=\"120\">100%</text>\n<text x=\"30\" y=\"160\">75%</text>\n<text "
            "x=\"30\" y=\"200\">50%</text>\n<text x=\"30\" "
            "y=\"240\">25%</text>\n<text x=\"30\" y=\"280\">0%</text>\n</g>\n";
    file << "<!-- Линии горизонтальной сетки (опционально) -->\n<g "
            "stroke=\"#ddd\" stroke-width=\"1\">\n<line x1=\"40\" y1=\"120\" "
            "x2=\"3460\" y2=\"120\" />\n<line x1=\"40\" y1=\"160\" x2=\"3460\" "
            "y2=\"160\" />\n<line x1=\"40\" y1=\"200\" x2=\"3460\" y2=\"200\" "
            "/>\n<line x1=\"40\" y1=\"240\" x2=\"3460\" y2=\"240\" />\n<line "
            "x1=\"40\" y1=\"280\" x2=\"3460\" y2=\"280\" />\n</g>\n";

    int i = 1, x = 60;
    file << "<!-- Столбцы -->\n<g fill=\"#4e79a7\">\n";
    for (auto it = data.begin(); it != data.end(); ++it, ++i, x += 80) {
      double height = 180 * (*it) / 100;
      file << "<rect x=\"" << x << "\"  y=\"" << 280 - height
           << "\"  width=\"60\" height=\"" << height << "\" />\n";
    }
    file << "</g>\n";

    x = 90;
    file << "<!-- Подписи категорий (снизу) -->\n<g font-size=\"12\" "
            "fill=\"#333\" text-anchor=\"middle\">\n";
    for (unsigned int i = 0; i < data.size(); i++, x += 80) {
      file << "<text x=\"" << x << "\"  y=\"310\">" << (char)(i + 65)
           << " </text>\n";
    }
    file << "</g>\n\n";
  }
};

class view3 : public View {
public:
  void update(std::list<double> data, std::fstream &file) override {
    file << "<script>\nconst data = [\n";
    double massiv[data.size()];
    double max = 0;
    int i = 0;

    for (auto it = data.begin(); it != data.end(); ++it, i++) {
      massiv[i] = *it;
      max += *it;
    }

    int q = 1, w = 1, e = 1;
    for (int j = 0; j < i; j++) {
      file << "{value: " << massiv[j] / max;
      file << ", color: '#" << q << q << w << w << e << e << "', label: '";

      if (q == w && w == e)
        q++;
      else if (w == e)
        w++;
      else
        e++;

      file << (char)(j + 65) << "'}";
      if (j != i - 1)
        file << ",\n";
      else
        file << std::endl;
    }
    file << "];\nconst svg = document.getElementById('pie');\nconst cx = 150, "
            "cy = 450, r = 120;\nlet sum = data.reduce((s, d) => s + d.value, "
            "0);\nlet startAngle = -Math.PI / 2;\ndata.forEach((d, i) => "
            "{\nconst angle = (d.value / sum) * 2 * Math.PI;\nconst endAngle = "
            "startAngle + angle;\nconst x1 = cx + Math.cos(startAngle) * "
            "r;\nconst y1 = cy + Math.sin(startAngle) * r;\nconst x2 = cx + "
            "Math.cos(endAngle) * r;\nconst y2 = cy + Math.sin(endAngle) * "
            "r;\nconst largeArc = angle > Math.PI ? 1 : 0;\nconst path = `M "
            "${cx},${cy} L ${x1},${y1} A ${r},${r} 0 ${largeArc},1 ${x2},${y2} "
            "Z`;\nsvg.innerHTML += `<path d=\"${path}\" "
            "fill=\"${d.color}\"/>`;\nstartAngle = endAngle;\n});\n</script>\n";
  }
};

class Model {
private:
  View *views[3];

public:
  Model() {
    views[0] = new view1();
    views[1] = new view2();
    views[2] = new view3();
  }
  ~Model() {
    delete views[0];
    delete views[1];
    delete views[2];
  }

  void update(std::list<double> data, std::fstream &file) {
    file.clear();
    file.seekp(0);
    file << "<!DOCTYPE "
            "html>\n<html>\n<head>\n<title>SVG-графика</title>\n</"
            "head>\n<body>\n<svg id=\"pie\" width=\"10000\" height=\"10000\">"
         << std::endl;
    for (int i = 1; i >= 0; i--) {
      views[i]->update(data, file);
    }
    file << "</svg>\n" << std::endl;
    views[2]->update(data, file);
    file << "</body>\n</html>" << std::endl;
  }
};

class Controller {
private:
  Model *model;
  std::fstream file;
  std::fstream file_out;
  std::list<double> data;
  std::string &in;
  std::string &out;

public:
  Controller(std::string &in, std::string &out) : in(in), out(out) {
    file.open(in, std::ios::in | std::ios::out | std::ios::app);
    file_out.open(out, std::ios::out | std::ios::trunc);
    if (!file.is_open() || !file_out.is_open()) {
      throw std::runtime_error("Failed to open files");
    }
    model = new Model();
    file.close();
  }

  ~Controller() {
    delete model;
    data.clear();
  }
  
  void update() {
    std::cout << "draw..." << std::endl;
    data.clear();
    file.open(in, std::ios::in | std::ios::out | std::ios::app);
    double value;
    while (file >> value) {
      data.push_back(value);
    }
    file.close();
    if (!data.empty()) {
      data.sort();
      file_out.close();
      file_out.open(out, std::ios::out | std::ios::trunc);
      model->update(data, file_out);
    }
    Sleep(10 * 1000);
    update();
  }
};

int main() {

  //_setmode(_fileno(stdout), _O_U16TEXT);
  /*Math *m[4] = {new Brackets, new Devide, new Integral, new Brackets};
  // m[0]->add(m[1]);
  // m[1]->add(m[2]);

  int x = 1000, y = 1000;
  std::wcout << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << 10000
             << "\" fill=\"" << "black"
             << "\" stroke-width=\"" << 3 << "\" stroke=\"rgb(0,0,0)\" />\n";

  m[0]->add(m[1]);
  m[0]->add(m[2]);
  m[0]->add(m[3]);
  m[0]->draw(x, y, 3000);*/

  std::string in1 = "C:/Users/lllnightlll/vscode/LabForEpstein/file1.txt";
  std::string out1 = "C:/Users/lllnightlll/vscode/LabForEpstein/file_out1.html";
  Controller c1(in1, out1);
  c1.update();
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
