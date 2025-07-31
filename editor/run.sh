set -e

BLUE="\e[36m"
NC="\e[0m"


echo -e "${BLUE} building project...${NC}"
mkdir -p build
cd build
cmake ..
make 

echo -e "${BLUE} running script...${NC}"
./editor

cd ..
