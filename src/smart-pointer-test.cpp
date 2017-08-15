#include "smart-pointer.h"

int main()
{
    int *i = new int(2);
    {
        ECSmartPointer<int> ptr1(i);
        {
            ECSmartPointer<int> ptr2(ptr1);
            {
                ECSmartPointer<int> ptr3 = ptr2;

                std::cout << *ptr1 << std::endl;
                *ptr1 = 20;
                std::cout << *ptr2 << std::endl;

            }
        }
    }
    return 0;
}

