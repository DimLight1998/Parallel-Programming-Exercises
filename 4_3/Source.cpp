// Implement trapezoidal rule using semaphore.

#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <cmath>

using namespace std;

class Semaphore
{
public:
    explicit Semaphore(const int count): _count(count)
    {
    }

    void Notify()
    {
        unique_lock<mutex> lck(_mutex);
        _count++;
        _cv.notify_one();
    }

    void Wait()
    {
        unique_lock<mutex> lck(_mutex);
        while (_count == 0) _cv.wait(lck);
        _count--;
    }

private:
    int _count;
    mutex _mutex;
    condition_variable _cv;
};

void CalculateAndAdd(const function<double(double)>& func, const double left, const double right,
                     const int numIntervals,
                     double* value, Semaphore& semaphore)
{
    const auto step = (right - left) / numIntervals;
    auto sum = 0.0;
    for (auto i = 0; i < numIntervals; i++)
    {
        const auto leftVal = func(left + i * step);
        const auto rightVal = func(left + (i + 1) * step);
        sum += (leftVal + rightVal) * step / 2;
    }

    semaphore.Wait();
    (*value) += sum;
    semaphore.Notify();
}

int main()
{
    // Calculate Integrate[Log[x + 2], {x, 0, 100}].
    Semaphore semaphore(1);

    thread threads[10];
    auto sum = 0.0;
    for (auto i = 0; i < 10; i++)
    {
        threads[i] = thread(&CalculateAndAdd,
                            [](const double x)-> double { return log(2 + x); },
                            i * 10, (i + 1) * 10, 2048, &sum, ref(semaphore));
    }
    for (auto& t : threads)
    {
        t.join();
    }

    cout << "Value is " << sum << endl;

    getchar();
    getchar();
    getchar();
    getchar();
    getchar();

    return 0;
}
