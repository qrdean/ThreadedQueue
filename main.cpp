//Quinton Dean
//Fall 2014 Final Project
//COSC 2336

//All commented "couts" are for flow purposes ONLY

#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <thread>
#include <random>
#include <mutex>
using namespace std;
using namespace std::chrono;
bool stop=false;
void Sleep(int x);
void f(int y);
int setInterval();
int Random();
mutex lock1;
//#of lanes
int SIZE=4;         //change number to change number of lanes
int total=0;
int served=0;
typedef queue<int> queue1;
vector<queue1> lanes(SIZE);
//counting customers
vector <int> customer(SIZE);

/*This program checks people out in a grocery store using queues. It is run in parallel threads
*/

int main()
{
    int i;
    int j;
    int minimum=0;
    //initializing counter and queues
    for(i=0; i<SIZE; i++)
    {
        customer.at(i)=0;
        for(j=0; j<10; j++)
        {
            lanes[i].push(setInterval());
            total++;
        }
    }
    //initializing thread
    thread t[SIZE];
    for(i=0; i<SIZE; i++)
    {
        //cout << "thread" << '\n';
        t[i]=thread (f, i);
    }

    // new customers coming in
    for(i=0; i<10; i++)
    {

        Sleep(setInterval());
        lock1.lock();
        for(j=0; j<SIZE; j++)
        {
            //cout << lanes[j].size() <<'\n';
        if(lanes[j].size() < lanes[minimum].size())
        {

            minimum = j;

        }
        }
        lanes[minimum].push(setInterval());
        total++;
        lock1.unlock();

    }
    stop=true;
    //joining threads and waiting for threads to end
    for(i=0; i<SIZE; i++)
    {
        //cout << "join" << '\n';
        t[i].join();
    }
    cout << "Lane " << setw(15) << "Customers served" << setw(15) << "Left in Queue" << '\n';
    //output
    for(i=0; i<SIZE; i++)
    {

        cout << i+1 << setw(10) << customer[i] << setw(15) << lanes[i].size() << '\n';
    }
    cout << "Total in queue " << setw(10) << total << '\n';
    cout << "Total served" << setw(10) << served << '\n';

}

void f(int y)
{
    while(true)
    {
        if(stop)
        {
            return;
        }
        while(!lanes[y].empty())
        {
            if(stop)
            {
                return;
            }
            //cout << "lane number " << y << " " << lanes[y].size() <<'\n';
            Sleep(lanes[y].front());
            lock1.lock();
            customer[y]++;
            served++;
            lock1.unlock();
            //cout << "served " << customer[y] << '\n';
            lanes[y].pop();
            //cout << "size " <<lanes[y].size() << '\n';
        }
        Sleep(1);
        //cout <<"hi";
        break;
    }

}

void Sleep(int x)

{
    std::this_thread::sleep_for(std::chrono::seconds(x));
}

int setInterval()
{
    return 5;               //   comment this for phase 2
//  return Random();        //     Uncomment this for phase 2
}

int Random ()
{
static int i=0;
static std::random_device rd;
static std::mt19937_64 gen(rd());
static std::uniform_int_distribution<int> dis(1,5);
    if (i==0)
    {
        i=1;
       milliseconds ms = duration_cast< milliseconds >(
high_resolution_clock::now().time_since_epoch());
gen.seed(ms.count());

    }
   return dis(gen);
}
