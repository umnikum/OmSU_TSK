#include <iostream>
#include <string>                   //Регулярно пользуюсь строками, и всем рекомендую

using namespace std;

class Currency{                     //Виртуальный класс валюты, кстати рекомендую перенести его с наследниками в заголовочный .h файл
    public:
        virtual double toRubles()const{return 0.0;}; //Заглушки в случае использования функций у инициальзованного объекта валюты
        virtual void print()const{cout << "Function print() is not overloaded!";};//const означает неизменность параметров класса при вызове
    protected:
        double value;               //Добавил полезные поля для наследников чтобы там их не определять, private такое не позволит
        double exchangeRate;
        string sign;
};

class Dollar: public Currency{      //Доллар реализует только свой конструктор и переопределяет виртуаьные функции
    public:
        Dollar(double value);
        double toRubles()const;
        void print()const;
};

Dollar::Dollar(double value = 0.0){
    this->value = value;
    exchangeRate = 100.0;           //Перевод от балды, курсы не знаю
    sign = "$";
}

double Dollar::toRubles()const{     //Переопределение функций
    return value * exchangeRate;
}

void Dollar::print()const{
    cout << value << ' ' + sign;
}

class Euro: public Currency{    
    public:
        Euro(double value);
        double toRubles()const;
        void print()const;
};

Euro::Euro(double value = 0.0){     //Аналогично для евро, ноз его знак 2символьный поэтому поместился только в строку
    this->value = value;
    exchangeRate = 120.0;
    sign = "€";
}

double Euro::toRubles()const{
    return value * exchangeRate;
}

void Euro::print()const{
    cout << value << ' ' + sign;
}

int main()
{
    //TODO мой стиль консоли весьма узнаваем, интерфейс уж постарайся сделать сам! Примеры использования классов ниже:
    Dollar hollaHollaGetADolla(10.0);
    Euro grannysGift(100.0);
    hollaHollaGetADolla.print();
    cout << " in rubles: " << hollaHollaGetADolla.toRubles() << endl;
    grannysGift.print();
    cout << " in rubles: " << grannysGift.toRubles();
    return 0;
}

