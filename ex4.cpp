// Chapter 19, exercise 04: redo exercise 13 from Chapter 17 with Link<God>.

#include "std_lib_facilities.h"

struct God
{
    string name;
    string country;
    string vehicle;
    string weapon;
    God(const string &n, const string &c, const string &v, const string &w)
        : name{n}, country{c}, vehicle{v}, weapon{w}
    {
    }
};

bool operator==(const God &god1, const God &god2)
{
    return god1.name == god2.name && god1.country == god2.country &&
           god1.vehicle == god2.vehicle && god1.weapon == god2.weapon;
}

bool operator>(const God &god1, const God &god2)
{
    return god1.name > god2.name;
}

//----------------------------------------------------------------

template <typename T>
class Link
{
public:
    T obj;
    Link(const T &t, Link *p = nullptr, Link *s = nullptr)
        : obj{t}, prev{p}, succ{s}
    {
    }
    Link *insert(Link *n);
    Link *add(Link *n);
    Link *add_ordered(Link *n);
    Link *erase();
    Link *find(const T &t);
    const Link *find(const T &t) const;
    Link *advance(int n);
    Link *next() const { return succ; };
    Link *previous() const { return prev; };

private:
    Link *prev;
    Link *succ;
};

template <typename T>
Link<T> *Link<T>::insert(Link<T> *n)
{
    if (n == nullptr)
        return this;
    if (this == nullptr)
        return n;
    n->succ = this;
    if (prev)
        prev->succ = n;
    n->prev = prev;
    prev = n;
    return n;
}

template <typename T>
Link<T> *Link<T>::add(Link<T> *n)
{
    if (n == nullptr)
        return this;
    if (this == nullptr)
        return n;
    n->prev = this;
    if (succ)
        succ->prev = n;
    n->succ = succ;
    succ = n;
    return n;
}

template <typename T>
Link<T> *Link<T>::add_ordered(Link<T> *n)
{
    if (n == nullptr)
        error("nullptr was sent to add_ordered()");

    Link *p = this;
    while (p)
        if (n->obj > p->obj)
        {
            if (p->succ == nullptr)
            {
                p->add(n);
                break;
            }
            else
                p = p->succ;
        }
        else
        {
            p->insert(n);
            break;
        }
    //set pointer "p" to the first position and return it
    while (p->prev)
        p = p->prev;

    return p;
}

template <typename T>
Link<T> *Link<T>::erase()
{
    if (this == nullptr)
        return nullptr;
    if (succ)
        succ->prev = prev;
    if (prev)
        prev->succ = succ;
    return succ;
}

template <typename T>
Link<T> *Link<T>::find(const T &t)
{
    Link *p = this;
    while (p)
    {
        if (p->obj == t)
            return p;
        p = p->succ;
    }
    return nullptr;
}

template <typename T>
Link<T> *Link<T>::advance(int n)
{
    Link *p = this;
    if (p == nullptr)
        return nullptr;
    if (0 < n)
        while (n--)
        {
            if (p->succ == nullptr)
                return nullptr;
            p = p->succ;
        }
    else if (n < 0)
        while (n++)
        {
            if (p->prev == nullptr)
                return nullptr;
            p = p->prev;
        }
    return p;
}

void print_all(Link<God> *p)
{
    while (p)
    {
        cout << "Имя: " << p->obj.name << ".\nМесто: " << p->obj.country
             << ".\nТранспортное средство: " << p->obj.vehicle << ".\nОружие: " << p->obj.weapon << ".";
        if (p = p->next())
            cout << "\n\n";
    }
    cout << "\n\n-------------------------------------------\n\n";
}

int main()
try
{
    Link<God> *norse_gods = new Link{God{"Тор", "Асгард", "Летающая колесница", "Молот Мьёльнир"}};
    norse_gods = norse_gods->add_ordered(new Link{God{"Хеймдалль", "Химинбьёрг", "Лошадь с золотой гривой - Гулльтоп", "Золотой рог - Гьяллархорн"}});
    norse_gods = norse_gods->add_ordered(new Link{God{"Фрейр", "Асгард, Уппсала", "Корабль - Скидбладнир", "Меч света"}});
    norse_gods = norse_gods->add_ordered(new Link{God{"Локи", "Ётунхейм", "Превращение в любое животное/существо/человека", "Изменение формы объекта"}});
    norse_gods = norse_gods->add_ordered(new Link{God{"Улль", "Тисовая долина - Идалир", "Лыжи, сани", "Лук, меч"}});

    Link<God> *greek_gods = new Link{God{"Афина", "Аттика", "Колесница", "Копьё"}};
    greek_gods = greek_gods->add_ordered(new Link{God{"Посейдон", "Дно моря", "Колесница", "Трезубец"}});
    greek_gods = greek_gods->add_ordered(new Link{God{"Зевс", "Олимп", "Колесница", "Молния"}});
    greek_gods = greek_gods->add_ordered(new Link{God{"Аполлон", "Олимп, Делос", "Колесница", "Лук, стрелы"}});
    greek_gods = greek_gods->add_ordered(new Link{God{"Гера", "Аргос, Олимп", "Колесница", "Диадема"}});

    Link<God> *veda_gods = new Link{God{"Сурья", "Небеса", "Золотая колесница", "Изливание света и сияния"}};
    veda_gods = veda_gods->add_ordered(new Link{God{"Варуна", "Мировые воды", "Морское чудовище - Макара", "Аркан (петля) из змеи"}});
    veda_gods = veda_gods->add_ordered(new Link{God{"Брихаспати", "Планета Юпитер", "Колесница Закона", "Дубина грома, лук, стрелы, золотой топор"}});
    veda_gods = veda_gods->add_ordered(new Link{God{"Вишну", "Океан космоса во Вселенной", "Птица - Гаруда", "Булава"}});
    veda_gods = veda_gods->add_ordered(new Link{God{"Индра", "Небесное царство (рай) - Сварга", "Белый небесный слон - Айравата", "Копьё"}});

    print_all(norse_gods);
    print_all(greek_gods);
    print_all(veda_gods);
}
catch (const std::exception &e)
{
    std::cerr << "Exception: " << e.what() << '\n';
    return 1;
}
catch (...)
{
    cerr << "Some kind of exception\n";
    return 2;
}
