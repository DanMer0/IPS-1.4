#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

using namespace std;

double f(double x)
{
    return (4 / (1 + x * x));
}

double simpsons()
{
    double a = 0;
    double b = 1;
    double eps = 0.0001;
    double I = eps + 1, I1 = 0;//I-предыдущее вычисленное значение интеграла, I1-новое, с большим N.
    for (int N = 2; (N <= 4) || (fabs(I1 - I) > eps); N *= 2)
    {
        double h, sum2 = 0, sum4 = 0, sum = 0;
        h = (b - a) / (2 * N);//Шаг интегрирования.
        for (int i = 1; i <= 2 * N - 1; i += 2)
        {
            sum4 += f(a + h * i);//Значения с нечётными индексами, которые нужно умножить на 4.
            sum2 += f(a + h * (i + 1));//Значения с чётными индексами, которые нужно умножить на 2.
        }
        sum = f(a) + 4 * sum4 + 2 * sum2 - f(b);//Отнимаем значение f(b) так как ранее прибавили его дважды. 
        I = I1;
        I1 = (h / 3) * sum;
    }
    cout << I1 << endl;
    return 0;
}


int main()
{
    setlocale(LC_ALL, "Russian");

    // Границы интегрирования.
    double a = 0;
    double b = 1;

    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    double n = 100;

    cout << "Метод Симпсона на двух потоках:" << endl;

    double area1 = 0;
    double area2 = 0;
    n = 1000000;
    t1 = chrono::high_resolution_clock::now();
    std::thread thr1(simpsons, -1, 0, n, std::ref(area1));
    std::thread thr2(simpsons, 0, 1, n, std::ref(area2));
    thr1.join();
    thr2.join();
    double area = area1 + area2;
    cout << area << endl;

    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    chrono::duration <double> durationsimpsons = (t2 - t1);
    cout << "Время подсчёта: " << durationsimpsons.count() << " секунд." << endl << endl;

    return 0;
}