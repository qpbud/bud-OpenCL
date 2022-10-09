git submodule update --init --recursive third_party/OpenCL-Headers
git submodule update --init --recursive third_party/googletest
git submodule update --init third_party/boost
cd third_party/boost
git submodule update --init --recursive libs/assert
git submodule update --init --recursive libs/config
git submodule update --init --recursive libs/smart_ptr
