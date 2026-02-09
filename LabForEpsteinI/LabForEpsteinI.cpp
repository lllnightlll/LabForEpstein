// LabForEpsteinI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <list>

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

class Figure : IComponent {
    int 
};

int main()
{
    IComponent* m[7] = { new Composite, new Composite,new Composite,new Composite,new Leaf,new Leaf,new Leaf};
    m[0]->add(m[1]);
    m[0]->add(m[2]);
    m[1]->add(m[3]);
    m[1]->add(m[6]);
    m[4]->add(m[4]);
    m[4]->add(m[5]);

    m[0]->draw();
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
