# HW1說明文件

**姓名** : 林友鈞
**學號** : E64061151
**系級** : 資訊4乙

## 開發環境

* OS : Ubuntu 18.04
* CPU : Intel(R) Core(TM) i5-9400F CPU @ 2.90GHz
* Memory : 16GB
* Programming Language(version) : C++ 14


## 程式執行時間**
8 GB檔案 : 301s
21GB檔案 : 967s


## 程式開發與使用說明

### 開發

我主要使用到的演算法是K-Way merge sort。一開始會先把原始檔案做切割成小檔案去做排序，其中分成兩個threads，主要的thread(main)做I/O讀取、次要的thread則是做sorting還有output。等到次要的thread都join之後就代表小檔案的sorting做完了。

接下來就是k-way merge sort的部分。我使用以下資料結構去儲存檔案
```cpp=
struct chunkFile{
    FILE * file; 
    int * readValues;
    unsigned int size;
    bool end;
    unsigned int current;
    char filename[20];
};

```
其中，readValues的部分是陣列，每次在做I/O時會從檔案讀取一塊資料出來做K-way merge。size則是readValues所讀取到的size。current則是當前readValues的位置(index)。當所有檔案都還是還沒讀到結尾時，每回合會從K個chunkFile中的readValues[current]選取最小的那一個輸出。被選中的chunkFile物件的current++

### 使用說明


```shell=
## Compile the code
$ make main

## run
$ ./main [input.txt]
```

---


## 效能分析報告

### 優化說明

我總共寫了3種版本，其中Version 0因為程式架構爛導致排序速度很差，因此不針對其做優化。主要針對Version 1做優化，因此Version 1以及Version 2兩者架構相近，優化上主要針對I/O部分做優化，詳見以下各個版本的說明。

#### Version 0

這個是我最一開始寫的版本，在merge的過程很簡單，使用的是2-way merge。運算的過程也僅僅只是把number從file讀出來，做比較、write out，在運算過程中發現CPU使用效率的曲線起起落落，所以研判應該是花太多時間在I/O上

#### Version 1

這個版本跟上一個版本最大的不同在於我把input跟merge,write的部分分開，input自己一個thread，merge和write一個thread，形成以下架構：
> input : producer
> merge + write : consumer

演算法的部分仍然是使用2-way merge。兩個threads一起跑的結果比Version 0好一點，雖然CPU的效能幾乎都吃滿，但速度仍不如預期，主要問題應該還是I/O，畢竟2-way merge產生的暫存檔太多。因此我用火焰圖以及去觀察(附圖如下所示)。我發現他花了大部分的時間在write out。因此在優化上會優先從這裏著手。
![](https://i.imgur.com/pHz8n5D.png)



另外在Version 1中我有試著再把merge和write分開，各自獨立成一個thread。形成以下架構：

> input : producer
> merge : consumer + producer
> output : consumer

但苦於其中有些deadlock以及太過於依賴C++的queue所導致的記憶體錯誤，所以最後沒有成功實現。


#### Version 2

由於上個版本非常依賴I/O，加上這種大檔案的排序應屬於I/O Bound的程式，因此我決定從演算法的角度去降低I/O的次數。因此，我改採用K-way merge去實現。另外在此同時，我也發現``C++ fstream``的讀寫速度超級無敵慢，改採用較低階的fprintf以及fscanf後，I/O時間大幅減少。我再次使用火焰圖去觀察(附圖如下)。這次反而把大多的時間都花在sort上，算是有把I/O的時間減少。
![](https://i.imgur.com/5UJvD48.png)


### Version2 vs Version3 驗證效果如下

**1 GB檔案**
* v1 : 534s
* v2 : 44s

第二版在1GB就幾乎海放第一版，兩者相差了將近12倍。原因還是出在I/O時間上的差異，畢竟K-Way merge以及2-Way merge的演算法實現方式差距不大。

#### 使用perf stat結果如下：

##### Cache misses等數據
![](https://i.imgur.com/vcS48XX.png)


##### Branch misses等數據

![](https://i.imgur.com/9KlIb02.png)


**小結** : 透過演算法的修改，我大幅降低了I/O time，同時更減少了cache misses以及branch misses。也因為降低I/O time因此context-switches的數目也減少了。


## 結論

在同時運行3支程式後，我發現OS還蠻公平的，我的i5-9400F有6 cores，每支程式分到大約2個cores去執行。不過我發現當只運行一支程式時，OS會做contex-switch把程式換到另一個Core去執行(附圖如下)![](https://i.imgur.com/5ZQxg3e.png)
我其實沒有完全了解為何OS要這樣做，但我猜可能是OS要去保護CPU避免電流過大或是過熱等等。但我覺得這麼做並非必要，甚至我覺得OS應該要讓使用者承擔這項責任。

另外我覺得OS應該要再針對locality去做優化，由上面的Before After圖看來，即使是讀檔案相近位置的資料仍造成大量的cache miss發生。即使優化後cache miss rate還是將近50%。我覺得OS可以做的是在讀檔時把資料load進cache時可以每個檔案都放一小塊甚至是reference過的資料直接清掉。雖然這樣做可能對其他支程式而言是很糟的方式，但對我的程式應該會好很多。