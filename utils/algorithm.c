
void bubbleSortAscend(int *buf, unsigned int nNum) {
   int j, k;
   int flag = nNum;
   unsigned int tmp;
   while (flag > 0) {
      k = flag;
      flag = 0;
      for (j = 1; j < k; j++){
         if (a[j - 1] > a[j]) {
            tmp = a[j-1];
            a[j - 1] = a[j];
            a[j] = tmp;
            flag = j;
         }
      }
   }
}

int sum(int *buf,unsigned int nNum){
   unsigned int sum = 0;
   for (int i=0;i<nNum;i++) {
      sum += buf[i];
   }
   return sum;
}

