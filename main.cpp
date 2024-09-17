//#include <windows.h>
#include "Room.h"

using namespace std;

Room DoRoomCopy( Room R) //Функция для экспериментов с временными объектами
{ return R; }

int main()
{ 
   //SetConsoleOutputCP(1251);
   int max_rect;

   std::cout << "Enter max rect: ";
   std::cin >> max_rect;
   Room room1;
   Room room2;
   Room room3;
   Room room4;
   Room room5;

   room5 = room4 = room3 = room2 = room1;


   cin.get();
   cin.get();
	return 0;
}