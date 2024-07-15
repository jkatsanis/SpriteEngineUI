git submodule update --init --recursive

cd Engine/ThirdParty/sfml
git checkout 2.6.0
cd ../../../

mkdir -p Engine/Build
mkdir -p Engine/Template/Build
cd Engine/Scripts
./Setup.sh