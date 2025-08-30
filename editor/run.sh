set -e

BLUE="\e[36m"
NC="\e[0m"
ARGS=""

while getopts "cv" opt; do
   case $opt in
      c)
         echo "cleaning..."
         rm -rf build
         exit 
      ;;
   esac 
   case $opt in
   v) 
      echo "verbose enabled"
      ARGS+="-v"
      ;;
   esac 
done

echo -e "${BLUE} building project...${NC}"
mkdir -p build
cd build
cmake ..
make 

echo -e "${BLUE} running script...${NC}"
./editor $ARGS

cd ..
