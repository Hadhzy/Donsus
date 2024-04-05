struct TYPES{
  int a;
  char c;
};

TYPES return_multiple(){
  return {1, 'c'};
}

int main(){
   TYPES value = return_multiple();
}