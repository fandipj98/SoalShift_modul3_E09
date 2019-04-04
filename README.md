# Laporan Resmi Sistem Operasi Modul3 E09

#### Fandi Pranata Jaya - 05111740000056
#### Fadhil Musaad Al Giffary - 05111740000116

## Nomor 1
### Soal:
1. Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
<br/>Contoh:
<br/>./faktorial 5 3 4
<br/>3! = 6
<br/>4! = 24
<br/>5! = 120
### Jawaban:
Pertama - tama untuk menghitung faktorial secara parallel kita perlu membuat thread sebanyak parameter inputan yang di passing ke `int main()` dengan menggunakan syntax:
```
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[1005];

int main(int argc, char *argv[])
{
  t = argc-1;
  for (int i = 0; i < t; i++) {
    fact[i]=strtol(argv[i+1], NULL, 10);
    int err = pthread_create(&(tid[i]), NULL, &factorial, NULL);
    if (err != 0)
      printf("\n can't create thread : [%s]",strerror(err));
  }
}
```
Kemudian untuk menghitung faktorialnya digunakan fungsi seperti berikut:
```
int t, fact[1005], ans[1005];

void* factorial(void *arg)
{
	pthread_t id = pthread_self();
	for (int i = 0; i < t; i++) {
		if (pthread_equal(id, tid[i])) {
			ans[i]=1;
			for(int j=0; j<fact[i]; j++){
				ans[i]=ans[i]*(fact[i]-j);
			}
		}
	}
	return NULL;
}
```
Setelah itu agar hasilnya urut, maka perlu digunakan `pthread_join()` agar semua thread menunggu satu sama lain sampai semua selesai (return NULL) dan kemudian array inputan dan hasil disort dengan fungsi merge sort seperti syntax dibawah ini:
```
void merge(int arr[], int l, int m, int r) 
{ 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp arrays */
    int L[n1], R[n2]; 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        int m = l+(r-l)/2; 
  
        // Sort first and second halves 
        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 

int main(){
  mergeSort(fact,0,t-1);
  mergeSort(ans,0,t-1);
}
```
Kemudian tinggal print semua nilai yang ada di array yang sudah tersort secara urut.

## Nomor 3
### Soal:
3. Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:
<br/>a. Terdapat 2 karakter Agmal dan Iraj
<br/>b. Kedua karakter memiliki status yang unik
     * Agmal mempunyai WakeUp_Status, di awal program memiliki status 0
     * Iraj memiliki Spirit_Status, di awal program memiliki status 100
     * Terdapat 3 Fitur utama
       * All Status, yaitu menampilkan status kedua sahabat
         <br/>Ex: Agmal WakeUp_Status = 75
             <br/> Iraj Spirit_Status = 30
       * “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
       * “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point
     * Terdapat Kasus yang unik dimana:
       * Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)
       * Jika Fitur  “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)
     * Program akan berhenti jika Salah Satu :
       * WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)
       * Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”)
     * Syarat Menggunakan Lebih dari 1 Thread

### Jawaban:
Pertama - tama kita perlu membuat 3 thread untuk fitur "Agmal Ayo Bangun", "Iraj Ayo Tidur", "All Status". Kemudian membuat fungsi untuk ketiga thread tersebut dengan syntax seperti berikut:
```
void* agmalandiraj(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[0])) {
			if(inputflag==2){
				//printf("%d %d %d %d %d\n",flag1,flag2,counter1,counter2,inputflag);
				if(flag1==1){
					status[0]+=15;
					counter1++;
					if(counter1==3){
						flag2=0;
					}
					if (status[0] >= 100) {
						puts("Agmal Terbangun,mereka bangun pagi dan berolahraga");
						exit(0);
					}
				}
				inputflag = 0;
			}else{
				if(counter2>=3){
					sleep(10);
					puts("Agmal Ayo Bangun enable again");
					counter2=counter2-3;
					flag1=1;
					inputflag = 0;
				}
			}

		} else if (pthread_equal(id, tid[1])) {
			if(inputflag==3){
				//printf("%d %d %d %d %d\n",flag1,flag2,counter1,counter2,inputflag);
				if(flag2==1){
					status[1]-=20;
					counter2++;
					if(counter2==3){
						flag1=0;
					}
					if (status[1] <= 0) {
						puts("Iraj ikut tidur, dan bangun kesiangan bersama Agmal");
						exit(0);
					}
				}
				inputflag = 0;
			}else{
				if(counter1>=3){
					sleep(10);
					puts("Iraj Ayo Tidur enable again");
					counter1=counter1-3;
					flag2=1;
					inputflag = 0;
				}
			}

		} else if (pthread_equal(id, tid[2])){
			if(inputflag==1){
				inputflag = 0;
				printf("Agmal WakeUp_Status: %d\nIraj Spirit_Status: %d\n", status[0], status[1]);
			}
		}
	}
	return NULL;
}
```
Fungsi tersebut perlu di looping terus agar thread dapat berjalan terus menerus sampai kondisi tertentu. Kemudian di `int main()` juga perlu dilakukan looping untuk menerima inputan user berulang kali seperti syntax berikut ini:
```
int main(){
  while (1) {		

      scanf(" %[^\t\n]s",inputString);

      //printf("%s\n",inputString);
      if (strcmp(inputString, "All Status") == 0) {

        inputflag = 1;

      } else if (strcmp(inputString, "Agmal Ayo Bangun") == 0){

        inputflag = 2;
        if(counter2>=3){
          puts("Agmal Ayo Bangun disabled 10 s");
        }

      } else if (strcmp(inputString, "Iraj Ayo Tidur") == 0){

        inputflag = 3;
        if(counter1>=3){
          puts("Fitur Iraj Ayo Tidur disabled 10 s");
        }

      }

  }
}
```
Jika inputan user adalah "Agmal Ayo Bangun", maka akan menambah WakeUp_Status Agmal sebesar 15, jika inputan user adalah "Iraj Ayo Tidur", maka akan mengurangi Spirit_Status Iraj sebanyak 20, jika inputan user adalah "All Status", maka akan menampilkan WakeUp_Status dari Agmal dan Spirit_Status dari Iraj.
Jika fitur Agmal Ayo Bangun dijalankan, maka counter1 akan ditambah 1, Jika fitur Iraj Ayo Tidur dijalankan, maka counter2 akan ditambah 1. Jika counter1>=3 maka counter1 akan dikurangi 3 dan fitur Iraj Ayo Tidur akan didisable selama 10 detik. Jika counter2>=3 maka counter2 akan dikurangi 3 dan fitur Agmal Ayo Bangun akan didisable selama 10 detik.
Program akan berakhir jika WakeUp_Status Agmal >=100 dan Spirit_Status Iraj <=0.
