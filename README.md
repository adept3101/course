# CLI Currency Converter in C
It's mostly written in C, but I used Go for the server and for converting XML to JSON.

# Usage
1. Clone repository:
```
git clone https://github.com/adept3101/course
cd course
```
2. Clone and build cJSON lib:
```
git clone https://github.com/DaveGamble/cJSON
cd cJSON
mkdir build
cd build
cmake ..
make
```

3. Build app:```make```
4. Run Go server: ```go run go/main.go``` and run app ```./main```
