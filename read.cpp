// reading a text file
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Point { 
   int x, y; 
};


int main () {
  Point point[15];
  string line;
  int n_of_object = 5;
  int count = 0;
  int n_of_point, x, y;
  ifstream file("building.txt");

  if (file.is_open()){
      while (count < n_of_object && file >> n_of_point){
        cout<< n_of_point<<endl;
        for(int i=0;i<n_of_point;i++){
          file >> x;
          file >> y;
          point[i].x = x;
          point[i].y = y;
          printf("%d %d\n",point[i].x, point[i].y);
        }
        // create_polygon(size,point);
        count++;
      } 
      file.close();
  }

  else cout << "Unable to open file"; 

  return 0;
}