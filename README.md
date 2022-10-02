# Search engine

Application for searching queries in local files.

### How it works
1. User puts files location in `config.json`.
2. User enters queries in `request.json`.
3. User gets results in `answers.json`.

- You can change max responces in `config.json`.
- The structure of the `answer.json` file:
1. `request[N]` - request number N in `request.json`.
2. `relevance` contains:
- `docid` - number of .txt file in `config.json`.
- `rank` - relative rank considering other files.
3. `result` - found or not found.

## Third party
- [JSON](https://github.com/nlohmann/json) - JSON for modern C++.
- [Google test](https://github.com/google/googletest) - unit tests.

## How to build searchEngine.
1. Create directory for build:
```
mkdir build && cd build
```
2. Build search engine with CMake:
```
cmake .. && cmake --build . --config Release --target searchEngine
```
- The execution file will be placed in the bin directory.
## How to build googleTests.
1. Create directory for build:
```
mkdir build && cd build
```
2. Build search engine with CMake:
```
cmake .. && cmake --build . --config Release --target googleTests
```
- The execution file will be placed in the tests directory, but you can run tests by using command:
```
ctest
```
- in build directory.