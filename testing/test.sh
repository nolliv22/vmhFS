# Exit when any command fails
set -e

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

# Generating file
dd if=/dev/zero of=/tmp/6mb bs=1M count=6
echo "Random string 123" > /tmp/foo.txt 

# Write your own test here


# PREDEFINED TESTS (uncomment to test)

# # Remove old file test
# $EXEC $FS write /tmp/foo.txt /foo.txt
# $EXEC $FS write /tmp/6mb /6mb1
# $EXEC $FS ls
# $EXEC $FS write /tmp/6mb /6mb2
# $EXEC $FS ls

# # Read file
# $EXEC $FS write /tmp/foo.txt /foo.txt
# $EXEC $FS ls
# $EXEC $FS read /foo.txt