# C++ HTTP-Server

## CMake Compiling

Replace {PATH_TO_SOURCE_CODE} with full path to source-code folder.


`cmake -DCMAKE_BUILD_TYPE=Debug -B/{PATH_TO_SOURCE_CODE}/cmake-build-debug -H/{PATH_TO_SOURCE_CODE}/`

`cmake --build /{PATH_TO_SOURCE_CODE/cmake-build-debug --target simple-http-server -- -j 2`

## Usage

`simple-http-server -t %THREAD_MAX% %PORT%`
`simple-http-server -f %PORT%`

## Testing
 - HTML File response.
 - Directory listing.
 - CGI-BIN scripts.
 - Multi-threading check with GPRS throttling.
 - 404 response.
 - Proper Content-Type headers.
