
typedef struct point {
  long x;
  long y;
  long z;
  long w;
} point;

point return_point(long x, long y, long z, long w){
  point p;
  p.x = x;
  p.y = y;
  p.z = z;
  p.w = w;
  return p;
}

long maxx(long x, long y){
  if(x > y) return x;
  return y;
}

long recv_point(point p1, point p2){
  long res1 = maxx(maxx(p1.x, p1.y), maxx(p1.z, p1.w));
  long res2 = maxx(maxx(p2.x, p2.y), maxx(p2.z, p2.w));
  return maxx(res1, res2);
}

int main(){
  point p1 = return_point(11, 4, 5, 14);
  point p2 = return_point(19, 19, 8, 10);
  long res = recv_point(p1, p2);

  return res-19;
}