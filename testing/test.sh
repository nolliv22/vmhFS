# Go to current working directory
cd "$PWD"

# Intro
echo "------------------"
echo "vhmFS Unit Testing"
echo "------------------"

# Compile
echo "Compiling..."
make
echo "------------------"

# Variables
EXEC="./build/vhmFS"
FS="/tmp/tmpFS"

# Create a 10 MB file system
echo "Create a 10 MB file system"
$EXEC $FS create 10

# 