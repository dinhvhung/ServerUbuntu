# Cách cài đặt Server trên Google Colab

## Tạo thư mục mới trên Colab

Đảm bảo bạn đang ở thư mục content/ (kiểm tra bằng !pwd)
```
!mkdir server
%cd server
```

## Cài đặt các thư viện cần thiết

### Cài lần lượt các câu lệnh sau

```
!sudo apt-get update
!sudo apt-get install cmake g++ libboost-all-dev python3-dev python3-pip
!pip install pybind11-global numpy
!sudo apt-get install build-essential libboost-all-dev python3-dev python3-pybind11 
```
### Kéo code về
```
!git clone https://github.com/dinhvhung/ServerUbuntu.git
%cd ServerUbuntu
```

## Tạo file build để chạy code

```
!mkdir build
!cmake -B build ./ -DCMAKE_BUILD_TYPE=Release
!make -C build -j1
```
(Bạn có thể thay j1 = j2, j3, j4 nếu RAM đủ mạnh để tiết kiệm thời gian tải)

### Lệnh mẫu để chạy thử
```
!./build/lifelong --inputFile ./example_problems/random.domain/random_32_32_20_100.json -o test.json
```

## Chạy Server

```
%cd socket/
!g++ -O3 server.cpp -o server
!chmod +x ./server
!./server
```


