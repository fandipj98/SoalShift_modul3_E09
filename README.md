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

##Nomor 2
### Soal:
2. Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:
	* Terdapat 2 server: server penjual dan server pembeli 
	* 1 server hanya bisa terkoneksi dengan 1 client
	* Server penjual dan server pembeli memiliki stok barang yang selalu sama
	* Client yang terkoneksi ke server penjual hanya bisa menambah stok
		* Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1
	* Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok
		* Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1
	* Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok
		* Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”
		* Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”
	* Server penjual akan mencetak stok saat ini setiap 5 detik sekali
	* __Menggunakan thread, socket, shared memory__

## Jawaban:
Langkah pertama adalah membuat dua buah server untuk penjual dan pembeli yang memiliki suatu shared memory untuk memeriksa stock barang. Sintaks untuk keduanya adalah:
```
int *stock;
...
key_t key = 1234;

int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
stock = shmat(shmid, NULL, 0);
*stock = 0;
...

shmdt(stock);
shmctl(shmid, IPC_RMID, NULL);
```
Lalu, agar masing - masing server dapat terhubung dengan clientnya masing - masing maka dibuatkan socket untuk tiap server dengan port yang berbeda dalam hal ini, pembeli di assign dengan port 8080. Sintaks server pembeli menjadi:
```
#define PORT 8080

int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[10] = {0};
char *fail = "transaksi gagal";
char *success = "transaksi berhasil";
  
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
}
  
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
}

address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons( PORT );
  
if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
}

if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
}

if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
    perror("accept");
    exit(EXIT_FAILURE);
}
```
Sedangkan penjual akan diberikan port 8000. Sntaksnya menjadi:
```
int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[1024] = {0};
...
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
}
  
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
}

address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons( PORT );
  
if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
}

if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
}

if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
    perror("accept");
    exit(EXIT_FAILURE);
}
```
Dapat diperhatikan bahwa untuk server pembeli hanya perlu mengirimkan hasil transaksi gagal atau berhasil sekaligus mengurangi stok sehingga pada main programnya hanya memerlukan:
```
while (1) {
    valread = read( new_socket , buffer, 10);
    if (*stock <= 0) {
        send(new_socket , fail , strlen(fail) , 0 );
    } else {
        send(new_socket , success , strlen(success) , 0 );
        (*stock)--;
    }
}
```
Sedangkan untuk server penjual perlu menambah stok dan display stock setiap 5 detik secara paralel. Sehingga diperlukan multi threading
```
void* routine(void *arg)
{
    pthread_t id = pthread_self();
    if (pthread_equal(id, tid[0])) {
        while (1) {
            printf("Stock: %d\n", *stock);
            sleep(5);
        }
    } else {
        while (1) {
            valread = read( new_socket , buffer, 1024); 
            (*stock)++;
        }
    }

    return NULL;
}
...
pthread_create(&(tid[0]),NULL,&routine,NULL);
pthread_create(&(tid[1]),NULL,&routine,NULL);
pthread_join(tid[0],NULL);
pthread_join(tid[1],NULL);
```
Jika sudah, maka pembuatan server telah selesai. Untuk membuat client cukup membuat socket dengan port yang bersesuaian dengan servernya dan inputan masing - masing. Untuk client pembeli sintaksnya:
```
#define PORT 8080
...
struct sockaddr_in address;
int sock = 0, valread;
struct sockaddr_in serv_addr;
char input[10];
char buffer[1024] = {0};

if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
}

memset(&serv_addr, '0', sizeof(serv_addr));

serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(PORT);
  
if(inet_pton(AF_INET, "192.168.43.160", &serv_addr.sin_addr)<=0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
}

if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
}
while (1) {
    scanf("%s", input);
    send(sock , input , strlen(input) , 0 );
    memset(buffer, 0, sizeof buffer);
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
}
```
Sedangkan untuk penjual sintaksnya:
```
#define PORT 8000
struct sockaddr_in address;
int sock = 0, valread;
struct sockaddr_in serv_addr;
char input[10];
char buffer[1024] = {0};

if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
}

memset(&serv_addr, '0', sizeof(serv_addr));

serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(PORT);
  
if(inet_pton(AF_INET, "192.168.43.160", &serv_addr.sin_addr)<=0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
}

if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
}
while (1) {
    scanf("%s", input);
    send(sock , input , strlen(input) , 0 );
}
```


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

## Nomor 4
### Soal:


## Nomor 5
### Soal:
5. Angga, adik Jiwang akan berulang tahun yang ke sembilan pada tanggal 6 April besok. Karena lupa menabung, Jiwang tidak mempunyai uang sepeserpun untuk membelikan Angga kado. Kamu sebagai sahabat Jiwang ingin membantu Jiwang membahagiakan adiknya sehingga kamu menawarkan bantuan membuatkan permainan komputer sederhana menggunakan program C. Jiwang sangat menyukai idemu tersebut. Berikut permainan yang Jiwang minta.
<br/>a. Pemain memelihara seekor monster lucu dalam permainan. Pemain dapat  memberi nama pada monsternya.
<br/>b. Monster pemain memiliki hunger status yang berawal dengan nilai 200 (maksimalnya) dan nanti akan berkurang 5 tiap 10 detik.Ketika hunger status mencapai angka nol, pemain akan kalah. Hunger status dapat bertambah 15 apabila pemain memberi makan kepada monster, tetapi banyak makanan terbatas dan harus beli di Market.
<br/>c. Monster pemain memiliki hygiene status yang berawal dari 100 dan nanti berkurang 10 tiap 30 detik. Ketika hygiene status mencapai angka nol, pemain akan kalah. Hygiene status' dapat bertambah 30 hanya dengan memandikan monster. Pemain dapat memandikannya setiap 20 detik(cooldownnya 20 detik).
<br/>d. Monster pemain memiliki health status yang berawal dengan nilai 300. Variabel ini bertambah (regenerasi)daa 5 setiap 10 detik ketika monster dalam keadaan standby.
<br/>e. Monster pemain dapat memasuki keadaan battle. Dalam keadaan ini, food status(fitur b), hygiene status'(fitur c), dan ‘regenerasi’(fitur d) tidak akan berjalan. Health status dari monster dimulai dari darah saat monster pemain memasuki battle. Monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster pemain maupun NPC memiliki serangan sebesar 20. Monster pemain dengan monster musuh akan menyerang secara bergantian. 
<br/>f. Fitur shop, pemain dapat membeli makanan sepuas-puasnya selama stok di toko masih tersedia.
     * Pembeli (terintegrasi dengan game)
       * Dapat mengecek stok makanan yang ada di toko.
       * Jika stok ada, pembeli dapat membeli makanan.
     * Penjual (terpisah)
       * Bisa mengecek stok makanan yang ada di toko
       * Penjual dapat menambah stok makanan.

Spesifikasi program:
<br/>A. Program mampu mendeteksi input berupa key press. (Program bisa berjalan tanpa perlu menekan tombol enter)
<br/>B. Program terdiri dari 3 scene yaitu standby, battle, dan shop.
<br/>C. Pada saat berada di standby scene, program selalu menampilkan health status, hunger status, hygiene status, stok makanan tersisa, dan juga status kamar mandi (“Bath is ready” jika bisa digunakan, “Bath will be ready in [bath cooldown]s” jika sedang cooldown). Selain itu program selalu menampilkan 5 menu, yaitu memberi makan, mandi, battle, shop, dan exit.
<br/>Contoh :<br/>
<br/>Standby Mode
<br/>Health : [health status]
<br/>Hunger : [hunger status]
<br/>Hygiene : [hygiene status]
<br/>Food left : [your food stock]
<br/>Bath will be ready in [cooldown]s
<br/>Choices
<br/>1. Eat
<br/>2. Bath
<br/>3. Battle
<br/>4. Shop
<br/>5. Exit<br/>
<br/>D. Pada saat berada di battle scene, program selalu menampilkan health status milik pemain dan monster NPC. Selain itu, program selalu menampilkan 2 menu yaitu serang atau lari.
<br/>Contoh :<br/>
<br/>Battle Mode
<br/>Monster’s Health : [health status]
<br/>Enemy’s Health : [enemy health status]
<br/>Choices
<br/>1. Attack
<br/>2. Run<br/>
<br/>E. Pada saat berada di shop scene versi pembeli, program selalu menampilkan food stock toko dan milik pemain. Selain itu, program selalu menampilkan 2 menu yaitu beli dan kembali ke standby scene.<br/>Contoh :<br/>
<br/>Shop Mode
<br/>Shop food stock : [shop food stock]
<br/>Your food stock : [your food stock]
<br/>Choices
<br/>1. Buy
<br/>2. Back<br/>
<br/>F. Pada program penjual, program selalu menampilkan food stock toko. Selain itu, program juga menampilkan 2 menu yaitu restock dan exit.<br/>Contoh :<br/>
<br/>Shop
<br/>Food stock : [shop food stock]
<br/>Choices
<br/>1. Restock
<br/>2. Exit<br/>
<br/>G. Pastikan terminal hanya mendisplay status detik ini sesuai scene terkait (hint: menggunakan system(“clear”))
### Jawaban:
Untuk program pembeli dan game, pertama - tama kita perlu membaca inputan untuk nama monster, kemudian membuat 5 thread dan fungsinya yaitu:
1. Thread untuk mengurangi secara otomatis hunger status sebesar 5 setiap 10 detik ketika standby mode, jika hunger status <= 0 maka exit program.
2. Thread untuk mengurangi secara otomatis hygiene status sebesar 10 setiap 30 detik ketika standby mode, jika hygiene status <= 0 maka exit program.
3. Thread untuk menambah hygiene status sebesar 15 ketika memilih option bath pada standby mode. Thread ini memiliki waktu cooldown 20 detik.
4. Thread untuk menambah secara otomatis health status sebesar 5 setiap 10 detik ketika standby mode.
5. Thread untuk user interface dari 3 mode (standby, battle, shop) dan battle mode. Pada battle mode, monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster user maupun NPC memiliki serangan sebesar 20. Monster user dengan monster musuh akan menyerang secara bergantian dengan diawali oleh user. Pada battle mode, user dapat memilih option menyerang atau melarikan diri. Battle mode akan dilooping terus dan akan kembali ke standby mode jika user melarikan diri atau monster lawan mati. Jika pada saat battle mode, health status dari monster user <= 0 maka exit program.
<br/>Berikut syntax dari 5 fungsi thread tersebut:
```
void* hunger(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[0])) {
			if(flag==1){	
				sleep(10);
				if(flag==1){
					hunger_stat-=5;
					if(hunger_stat<=0){
						system("clear");
						printf("Standby Mode\nMonster's Name: %s\nHealth : %d\nHunger : %d\nHygiene : %d\nFood Left : %d\nBath is ready\nChoices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n", nama, health_stat, hunger_stat, hygiene_stat, food_stock);
						puts("You died because of starvation");
						exit(0);
					}
				}
			}
		}
	}
	return NULL;
}

void* hygiene(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[1])) {
			if(flag==1){
				sleep(30);
				if(flag==1){
					hygiene_stat-=10;
					if(hygiene_stat<=0){
						system("clear");
						printf("Standby Mode\nMonster's Name: %s\nHealth : %d\nHunger : %d\nHygiene : %d\nFood Left : %d\nBath is ready\nChoices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n", nama, health_stat, hunger_stat, hygiene_stat, food_stock);
						puts("You died because of dirty");
						exit(0);
					}
				}
			}
		}
	}
	return NULL;
}

void* mandi(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[2])) {
			if(mandi_flag==1){
				if(hygiene_stat<=100 && cooldown==0){	
					cooldown=20;
					hygiene_stat+=15;
					if(hygiene_stat>=100){
						hygiene_stat=100;
					}
				}
				mandi_flag=0;
			}
			while(cooldown>0){
				sleep(1);
				cooldown--;	
			}
		}
	}
	return NULL;
}

void* regen(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[3])) {
			if(flag==1){	
				sleep(10);
				if(health_stat<300 && flag==1){	
					health_stat+=5;
					if(health_stat>=300){
						health_stat=300;
					}
				}
			}
		}
	}
	return NULL;
}

void* game(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[4])) {
			if(flag==1){
				if(cooldown==0){
					system("clear");
					printf("Standby Mode\nMonster's Name: %s\nHealth : %d\nHunger : %d\nHygiene : %d\nFood Left : %d\nBath is ready\nChoices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n", nama, health_stat, hunger_stat, hygiene_stat, food_stock);
					sleep(1);
				}
				else{
					system("clear");
					printf("Standby Mode\nMonster's Name: %s\nHealth : %d\nHunger : %d\nHygiene : %d\nFood Left : %d\nBath will be ready in %ds\nChoices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n", nama, health_stat, hunger_stat, hygiene_stat, food_stock, cooldown);
					sleep(1);
				}
			}
			else if(flag==2){
				monster_health=100;
				while(1){
					sleep(1);
					system("clear");
					printf("Battle Mode\nMonster's Name: %s\nMonster's Health : %d\nEnemy's Health : %d\nChoices\n1. Attack\n2. Run\n", nama, health_stat, monster_health);
					input = mygetch() - '0';
					if(input==1){
						monster_health-=20;
						sleep(1);
						system("clear");
						printf("Battle Mode: your's turn\nMonster's Name: %s\nMonster's Health : %d\nEnemy's Health : %d\nChoices\n1. Attack\n2. Run\n", nama, health_stat, monster_health);
						if(monster_health<=0){
							puts("You win!");
							flag=1;
							break;
						}
						sleep(1);
						health_stat-=20;
						if(health_stat<=0){
							health_stat=0;
							system("clear");
							printf("Battle Mode\nMonster's Name: %s\nMonster's Health : %d\nEnemy's Health : %d\n", nama, health_stat, monster_health);
							
							puts("You died because killed");
							exit(0);
						}
						system("clear");
						printf("Battle Mode: enemy's turn\nMonster's Name: %s\nMonster's Health : %d\nEnemy's Health : %d\n", nama, health_stat, monster_health);
					}
					else if(input==2){
						puts("You ran away!");
						flag=1;
						break;
					}
				}
			}
			else if(flag==3){
				system("clear");	
				printf("Shop Mode\nShop food stock : %d\nYour food stock : %d\nChoices\n1. Buy\n2. Back\n", shop_stock[0], food_stock);
				sleep(2);
			}
		}
	}
	return NULL;
}
```
Pada standby mode, user memiliki 5 option yaitu:
1. Eat yaitu menambah hunger status sebesar 15 dan mengurangi food stock sebesar 1 setiap kali makan. Hanya bisa dilakukan jika food stocknya ada (>0).
2. Bath yaitu menjalankan thread 3 yang menambah hygiene status sebesar 15.
3. Battle yaitu merubah mode menjadi battle mode.
4. Shop yaitu merubah mode menjadi shop mode.
5. Exit yaitu exit dari program.
<br/>Pada shop mode, user memiliki 2 option yaitu:
1. Buy yaitu menambah food stock sebanyak 1 dan mengurangi shop food stock sebanyak 1. Option ini hanya bisa dijalankan jika shop food stocknya ada (>0).
2. Back yaitu kembali ke standby mode.
<br/><br/>Berikut syntax dari standby mode dan shop mode:
```
int main(){
	while (1) {		
			if(flag==1){
				input = mygetch() - '0';
				if(input==1){
					if(food_stock>0){
						if(hunger_stat<=200){	
							hunger_stat+=15;
							food_stock--;
							if(hunger_stat>=200){
								hunger_stat=200;
							}
						}
					}
					else{
						puts("Your Food Stock is Empty");
						sleep(1);
					}
				}
				else if(input==2 && cooldown==0){
					mandi_flag=1;
				}
				else if(input==3){
					flag=2;
				}
				else if(input==4){
					flag=3;
				}
				else if(input==5){
					puts("You Exit The Game");
					exit(0);
				}
			}
			else if(flag==3){
				input = mygetch() - '0';
				if(input==1){
					if(shop_stock[0]>0){
						food_stock++;
						shop_stock[0]--;	
					}
					else{
						puts("The Shop is Out of Stock");
					}
				}
				else if(input==2){
					flag=1;
				}
			}
	}
}
```
Kemudian program dapat mendeteksi inputan berupa key press (program bisa berjalan tanpa perlu menekan tombol enter). Syntaxnya adalah seperti berikut ini:
```
#include<termios.h>
int mygetch ( void ) 
{
  int ch;
  struct termios oldt, newt;
  
  tcgetattr ( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
  
  return ch;
}

int input;

int main(){
	input = mygetch() - '0';
}
```
Digunakan shared memory untuk shop food stocknya sehingga nilai dari shop food stock pada pembeli dapat bernilai sama dengan penjual. Berikut syntaxnya:
```
#include<sys/shm.h>

int main(){
	key_t key = 1234;
	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	shop_stock = shmat(shmid, NULL, 0);
}
```
Untuk program penjual, digunakan shared memory untuk shop food stocknya sehingga nilai dari shop food stock pada penjual dapat bernilai sama dengan pembeli. Selain itu juga dibuat 1 thread untuk user interfacenya. Kemudian untuk inputannya dapat dibaca dengan key press saja seperti program pembeli. Terdapat 2 mode pada program penjual yaitu:
1. Restock yaitu untuk menambah shop food stock sebesar 1.
2. Exit yaitu untuk exit dari program penjual.
<br/><br/>Berikut syntax dari program penjual:
```
#include<stdio.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<termios.h>

pthread_t tid[1];
int *shop_stock;

int mygetch ( void ) 
{
  int ch;
  struct termios oldt, newt;
  
  tcgetattr ( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
  
  return ch;
}

void* shop(void *arg)
{
        pthread_t id = pthread_self();
        while (1) {
                if (pthread_equal(id, tid[0])) {
                        system("clear");
                        printf("Shop\nFood stock : %d\nChoices\n1. Restock\n2. Exit\n", shop_stock[0]);
                        sleep(1);
                }
        }
        return NULL;
}


void main()
{
        key_t key = 1234;
        int input;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        shop_stock = shmat(shmid, NULL, 0);

        pthread_create(&(tid[0]), NULL, &shop, NULL);
        
        while (1) {
                input = mygetch() -'0';
                
                if(input==1){
                        shop_stock[0] = shop_stock[0] + 1;
                }
                else if(input==2){
                        puts("You Exit The Shop");
                        exit(0);
                }
        }
        //shmdt(shop_stock);
        //shmctl(shmid, IPC_RMID, NULL);
}
```
