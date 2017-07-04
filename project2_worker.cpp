
#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <iomanip.h>

const int MAXSIZE = 50;   // максимална дължина на име и на вид работа
// дефиниция на клас Worker, определящ
// работник по име и заплащане за 1 час
class Worker
{
public: //публични член-променливи
    // канонично представяне на класа
    Worker(const char* = "", double = 0.0); //copy конструктор
    Worker(const Worker&); //copy конструктор
    ~Worker(); //деструктор
    Worker& operator=(const Worker&); // операция =
    // извеждане на данните за работник
    void print() const; // член-функция
    // функция за достъп до член-данната salary
    double get_Salary() const;

private: //скрити член-променливи
    char* name;              // име на работник
    double salary;           // заплащане за 1 час на работник
};

Worker::Worker(const char* n, double s) // copy конструктор
{
    name = new char[strlen(n)+1]; //взима дължината на string +1
    if(!name)
    {
        cout << "Грешка при заделяне на памет!\n";
        exit(1);
    }
    strcpy(name, n); // копира n в name
    salary = s;
}

Worker::Worker(const Worker& w) // конструктор за присвояване
{
    name = new char[strlen(w.name)+1]; // дължината на името +1
    if(!name) //ако няма име
    {
        cout << "Грешка при заделяне на памет!\n";
        exit(1); //изход
    }
    strcpy(name, w.name); // копира w.name в name
    salary = w.salary; //заплатата е равна на заплатата на работника
}

Worker::~Worker() // деструктор
{
    delete [] name; // извиква деструктора
}

Worker& Worker::operator=(const Worker& w) // предефиниране на оператора =
{
    if(this != &w)
    {
        delete [] name;
        name = new char[strlen(w.name)+1];
        if(!name)
        {
            cout << "Грешка при заделяне на памет!\n";
            exit(1); //изход
        }
        strcpy(name, w.name); //копира w.name в name
        salary = w.salary; //заплатата е равна на заплатата на работника
    }
    return *this; //връща указателя към себе си
}

void Worker::print() const  // извеждане на данните на работник
{
    cout << setw(20) << name
         << setw(12) << salary; //извежда името и заплатата с определена дължина
}

// член-функция за достъп до заплащането за 1 час
double Worker::get_Salary() const
{
    return salary; //връща заплатата
}

// дефиниция на класа HourlyWorker, определящ
// почасов работник като работник, извършващ един
// вид работа определено количество часове
class HourlyWorker : public Worker // клас HourlyWorker наследник на Worker
{
public:
    // канонично представяне на класа
    HourlyWorker(const char* = "", double = 0.0, const char* = "", int = 0); //copy конструктор
    HourlyWorker(const HourlyWorker&); //copy конструктор
    ~HourlyWorker(); //деструктор
    HourlyWorker& operator=(const HourlyWorker&); //операция =
    // намиране на седмичната заплата на почасов работник
    double WeekSalary() const;
    // извеждане на член-данните и седмичната заплата на почасов работник
    void print() const;
private:
    char* work;         // вид извършвана работа
    int hours;          // брой изработени часове
};

// конструктор HourlyWorker наследник на Worker
HourlyWorker::HourlyWorker(const char* n, double s,
                           const char* w, int h) : Worker(n, s)
{
    work = new char[strlen(w)+1];
    if(!work) //ако няма работа
    {
        cout << "Грешка при заделяне на памет!\n";
        exit(1); // изход
    }
    strcpy(work, w); //копира w в work
    hours = h; //часовете са равни на h
}

// конструктор за присвояване
HourlyWorker::HourlyWorker(const HourlyWorker& p) : Worker(p)
{
    hours = p.hours; // часовете са равни на брой часове от седмицата
    work = new char[strlen(p.work)+1];
    if(!work)
    {
        cout << "Грешка при заделяне на памет!\n";
        exit(1);
    }
    strcpy(work, p.work);
}

// деструктор
HourlyWorker::~HourlyWorker()
{
    delete [] work;
}

// предефиниране на оператора =
HourlyWorker& HourlyWorker::operator=(const HourlyWorker& p)
{
    if(this != &p)
    {
        Worker::operator=(p);
        delete [] work;
        hours = p.hours;
        work = new char[strlen(p.work)+1];
        if(!work)
        {
            cout << "Грешка при заделяне на памет!\n";
            exit(1);
        }
        strcpy(work, p.work);
    }
    return *this;
}

// намиране на седмичната заплата на почасов работник
double HourlyWorker::WeekSalary() const
{
    double x, y = get_Salary();
    if(hours <= 40)
        x = hours*y;
    else if(hours <= 60)
        x = 40*y + (hours-40)*1.5*y;
    else
        x = 40*y + 20*1.5*y + (hours-60)*2*y;
    return x;
}

// извеждане на член-данните и седмичната заплата на почасов работник с определена дължина
void HourlyWorker::print() const
{
    Worker::print();
    cout << setw(10) << hours
         << setw(20) << work
         << setw(10) << WeekSalary() << endl;
}

// дефиниция на класа SalariedWorker, определящ
// щатен работник като работник, извършващ един
// вид работа определено количество часове
class SalariedWorker : public Worker  //клас SalariedWorker наследник на Worker
{
public:
    // канонично представяне на класа
    SalariedWorker(const char* = "", double = 0.0, const char* = "", int = 0); // copy конструктор
    SalariedWorker(const SalariedWorker&); // copy конструктор
    ~ SalariedWorker(); //деструктор
    SalariedWorker& operator=(const SalariedWorker&); //член-функция за оператор =
    // член-функция за намиране на седмичната заплата на щатен работник
    double WeekSalary() const;
    // извеждане на член-данните и седмичната заплата на щатен работник
    void print() const;

private:
    char* work;         // вид извършвана работа
    int hours;          // брой изработени часове
};

// конструктор по подразбиране
SalariedWorker::SalariedWorker(const char* n, double s,
                               const char* w, int h)
                               : Worker(n, s)
{
    work = new char[strlen(w)+1];
    if(!work)
    {
        cout << "Грешка при заделяне на памет!\n";
        exit(1);
    }
    strcpy(work, w);
    hours = h;
}

// конструктор за присвояване
SalariedWorker::SalariedWorker(const SalariedWorker& p)
                               : Worker(p)
{
    hours = p.hours;
    work = new char[strlen(p.work)+1];
    if(!work)
    {
        cout << "Грешка при заделяне на памет!\n";
        exit(1);
    }
    strcpy(work, p.work);
}

// деструктор
SalariedWorker::~SalariedWorker()
{
    delete [] work;
}

// предефиниране на оператора =
SalariedWorker& SalariedWorker::operator=(const SalariedWorker& p)
{
    if(this != &p)
    {
        Worker::operator=(p);
        delete [] work;
        work = new char[strlen(p.work)+1];
        if(!work)
        {
            cout << "Грешка при заделяне на памет!\n";
            exit(1);
        }
        strcpy(work, p.work);
        hours = p.hours;
    }
    return *this;
}

// определяне на седмичната заплата на щатен работник
double SalariedWorker::WeekSalary() const
{
    return 40 * get_Salary(); //връща заплатата за 40 часа
}

// извеждане на член-данните и седмичната
// заплата на щатен работник
void SalariedWorker::print() const
{
    Worker::print();
    cout << setw(10) << hours
         << setw(20) << work
         << setw(10) << WeekSalary() << endl; // извежда часове, работа и седмична заплата с определена дължина
}

// създаване на масив, съдържащ данни за почасови работници
void Create_HourlyWorker_arr(int m, HourlyWorker**& arr)
{
    cout << "Създаване на масив с данни за"
            " почасовите работници \n";
    // Масивът е разположен в динамичната памет и елементите му
    // са указатели към обекти от клас HourlyWorker.
    arr = new HourlyWorker*[m];
    if(!arr)
    {
        cout << "Грешка при заделяне на памет!\n";
        exit(1);
    }
    for(int i = 0; i < m; i++) //външен цикъл
    {
        char n[MAXSIZE];
        cout << "Име на работник: ";
        cin.getline(n, MAXSIZE);    // въвеждане на името на работник
        double s;
        cout << "Заплащане за 1 час на работник: ";
        cin >> s;                   // въвеждане на заплащането за 1 час
        cin.ignore();               // прескачане на знаците за край на ред
        char w[MAXSIZE];
        cout << "Вид на извършвана работа: ";
        cin.getline(w, MAXSIZE);    // въвеждане на вида на работа
        int h;
        cout << "Брой часове на работа: ";
        cin >> h;                   // въвеждане на часовете на работа
        cin.ignore();               // прескачане на знаците за край на ред
        arr[i] = new HourlyWorker(n, s, w, h);
        if(!arr[i])
        {
            cout << "Грешка при заделяне на памет!\n";
            exit(1);
        }
    }
}

// извеждане на член-данните и седмичната заплата
// на почасовите работници на масива с определена дължина
void Print_HourlyWorker_arr(int n, HourlyWorker** arr)
{
    cout << "Списък на почасовите работници\n";
    cout << setw(20) << "Име"
         << setw(12) << "Запл./1 ч."
         << setw(10) << "Часове"
         << setw(20) << "Вид работа"
         << setw(10) << "Заплата\n";
    for(int i = 0; i < n; i++)
        arr[i]->print(); // извеждане на член-данните от масива
}

// сортиране в низходящ ред по размера на
// заплатата на елементите на масива
void Sort_HourlyWorker_arr(int n, HourlyWorker** arr)
{
    // сортиране по метода на пряката селекция
    for(int i = 0; i < n-1; i++)
    {
        int k = i; double max = arr[i]->WeekSalary();
        for(int j = i+1; j < n; j++)
        {
            if(arr[j]->WeekSalary() > max)
            {
                max = arr[j]->WeekSalary();
                k = j;
            }
        }
        HourlyWorker * ptr = arr[i];
        arr[i] = arr[k];
        arr[k] = ptr;
    }
}

// освобождаване на заетата от масива и елементите му памет
void Delete_HourlyWorker_arr(int n, HourlyWorker** arr)
{
    for(int i = 0; i < n; i++)
        delete arr[i];
    delete [] arr;
}

// създаване на масив, съдържащ данни за щатни работници
void Create_SalariedWorker_arr(int m, SalariedWorker**& arr)
{
    cout << "Създаване на масив с данни за"
        " щатните работници \n";
    // Масивът е разположен в динамичната памет и елементите му
    // са указатели към обекти от клас SalariedWorker.
    arr = new SalariedWorker*[m];
    if(!arr)
    {
        cout << "Грешка при заделяне на памет!\n";
        exit(1);
    }
    for(int i = 0; i < m; i++)
    {
        char n[MAXSIZE];
        cout << "Име на работник: ";
        cin.getline(n, MAXSIZE);    // въвеждане на името на работник
        double s;
        cout << "Заплащане за 1 час на работник: ";
        cin >> s;                   // въвеждане на заплащането за 1 час
        cin.ignore();               // прескачане на знаците за край на ред
        char w[MAXSIZE];
        cout << "Вид на извършвана работа: ";
        cin.getline(w, MAXSIZE);    // въвеждане на вида на работа
        int h;
        cout << "Брой часове на работа: ";
        cin >> h;                   // въвеждане на часовете на работа
        cin.ignore();               // прескачане на знаците за край на ред
        arr[i] = new SalariedWorker(n, s, w, h);
        if(!arr[i])
        {
            cout << "Грешка при заделяне на памет!\n";
            exit(1);
        }
    }
}

// извеждане на член-данните и седмичната
// заплата на щатните работници на масива
void Print_SalariedWorker_arr(int m, SalariedWorker** arr)
{
    cout << "Списък на щатните работници\n"; //извежда член-данните с определена дължина
    cout << setw(20) << "Име"
         << setw(12) << "Запл./1 ч"
         << setw(10) << "Часове"
         << setw(20) << "Вид работа"
         << setw(10) << "Заплата\n";
    for(int i = 0; i < m; i++) //външен цикъл
        arr[i]->print(); //извежда член-данните от масива
}

// сортиране в низходящ ред по размер на
// заплатата на елементите на масива
void Sort_SalariedWorker_arr(int m, SalariedWorker** arr)
{
    // сортиране по метода на пряката селекция
    for(int i = 0; i < m-1; i++)
    {
        int k = i;
        double max = arr[i]->WeekSalary();
        for(int j = i+1; j < m; j++)
        {
            if(arr[j]->WeekSalary() > max)
            {
                max = arr[j]->WeekSalary();
                k = j;
            }
        }
        SalariedWorker * ptr = arr[i];
        arr[i] = arr[k];
        arr[k] = ptr;
    }
}

// освобождаване на заетата от масива и елементите му памет
void Delete_SalariedWorker_arr(int n, SalariedWorker** arr)
{
    for(int i = 0; i < n; i++)
        delete arr[i];
    delete [] arr;
}

int main()
{
    // въвеждане на броя на елементите на масива
    // с данни за почасовите работници
    int n; cout << "n= "; cin >> n;
    cin.ignore();        // прескачане на знаците за край на ред
    // деклариране на масива с данни за почасовите работници
    HourlyWorker** hourly_arr = NULL;
    // създаване на масива с данни за почасовите работници
    Create_HourlyWorker_arr(n, hourly_arr);
    // извеждане на масива с данни за почасовите работници
    Print_HourlyWorker_arr(n, hourly_arr);
    // въвеждане на броя на елементите на масива
    // с данни за щатните работници
    int m; cout << "m= "; cin >> m;
    cin.ignore();        // прескачане на знаците за край на ред
    // деклариране на масива с данни за щатните работници
    SalariedWorker** salaried_arr = NULL;
    // създаване на масива с данни за щатните работници
    Create_SalariedWorker_arr(m, salaried_arr);
    cout << endl;
    // извеждане на масива с данни за щатните работници
    Print_SalariedWorker_arr(m, salaried_arr);
    // сортиране в низходящ ред по размерa на заплатата
    // на масива с данни за почасовите работници
    Sort_HourlyWorker_arr(n, hourly_arr);
    // извеждане на сортирания масив
    // с данни за почасовите работници
    Print_HourlyWorker_arr(n, hourly_arr);
    // сортиране в низходящ ред по размерa на заплатата
    // на масива с данни за щатните работници
    Sort_SalariedWorker_arr(m, salaried_arr);
    // извеждане на сортирания масив
    // с данни за щатните работници
    Print_SalariedWorker_arr(m, salaried_arr);
    // освобождаване на заетата от масива
    // hourly_arr и елементите му памет
    Delete_HourlyWorker_arr(n, hourly_arr);
    // освобождаване на заетата от масива
    // salaried_arr и елементите му памет
    Delete_SalariedWorker_arr(m, salaried_arr);
    return 0;
}
