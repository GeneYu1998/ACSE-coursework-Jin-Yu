#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <time.h>

using namespace std;

class point
{
    public:
    point(){}
    point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    ~point(){}

    void generate_random_point()
    {   
        this->x = rand()%11 - 5;
        this->y = rand()%15 - 7;
        this->z = rand()%5 - 2;
    }

    void subtraction(point right)
    {
        this->x -= right.x;
        this->y -= right.y;
        this->z -= right.z; 
    }


    double x;
    double y;
    double z;
};

class tetrahedron
{
    public:
    
    tetrahedron(point a, point b, point c, point d)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
    }

    ~tetrahedron(){}
    
    void compute_volume()
    {   
        point mr;
        this->a.subtraction(this->d);
        this->b.subtraction(this->d);
        this->c.subtraction(this->d);
        mr.x = this->b.y * this->c.z - this->b.y * this->c.x;
        mr.y = this->c.x * this->b.z - this->c.z * this->b.x;
        mr.z = this->b.x * this->c.y - this->c.x * this->b.y;
        volume = this->a.x * mr.x + this->a.y * mr.y + this->a.z * mr.z;
        volume = abs(volume);
    } 


    double volume;
    point a;
    point b;
    point c;
    point d;
};


int main()
{   
    cout << "Part 1: " << endl;
    srand((int)time(0));
    auto a = new point();
    a->generate_random_point();
    cout << "Point a: " << " " << (*a).x << " " << (*a).y << " " << (*a).z << endl;

    auto b = new point();
    b->generate_random_point();
    cout << "Point b: " << " " << (*b).x << " " << (*b).y << " " << (*b).z << endl;

    auto c = new point();
    c->generate_random_point();
    cout << "Point c: " << " " << (*c).x << " " << (*c).y << " " << (*c).z << endl;

    auto d = new point();
    d->generate_random_point();
    cout << "Point d: " << " " << (*d).x << " " << (*d).y << " " << (*d).z << endl;
    
    auto Tetrahedron1 = new tetrahedron(*a, *b, *c, *d);
    Tetrahedron1->compute_volume();
    cout << "The volume of the Tetrahedron: " << endl;
    cout << Tetrahedron1->volume << endl;

    delete a;
    delete b;
    delete c;
    delete d;
    delete Tetrahedron1;
    
    double average = 0;

    cout << "Part 2: (creating 10000 objects)" << endl;
    for (int i = 0; i < 10000; i++)
    {
        auto a = new point();
        auto b = new point();
        auto c = new point();
        auto d = new point();

        a->generate_random_point();
        b->generate_random_point();
        c->generate_random_point();
        d->generate_random_point();

        auto Tetrahedron = new tetrahedron(*a, *b, *c, *d);

        Tetrahedron->compute_volume();

        average += Tetrahedron->volume;
       
        if (i < 10 || i > 9990)
        {cout << "Tetrahedron " << i+1 << "\t" << "volume: "<< Tetrahedron->volume << endl;}
        if (i > 12 && i < 15)
        {cout << "..." << endl;}


        delete a;
        delete b;
        delete c;
        delete d;
        delete Tetrahedron;

    }

     cout << "total average volume: " << average/10000 << endl;
}
       