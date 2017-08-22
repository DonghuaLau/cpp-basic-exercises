#include "smart-pointer.h"

int main()
{
    int *i = new int(2);
    {
        ec::SmartPointer<int> ptr1(i);
        {
            ec::SmartPointer<int> ptr2(ptr1);
            {
                ec::SmartPointer<int> ptr3 = ptr2;

                std::cout << *ptr1 << std::endl;
                *ptr1 = 20;
                std::cout << *ptr2 << std::endl;

            }
        }
    }
    return 0;
}

