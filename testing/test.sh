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
# $EXEC $FS test

# PREDEFINED TESTS (please follow the order)

# # 1) WRITE: Overwrite old file
# $EXEC $FS write /tmp/foo.txt /foo.txt
# $EXEC $FS write /tmp/6mb /6mb1
# $EXEC $FS ls
# $EXEC $FS write /tmp/6mb /6mb2
# $EXEC $FS ls

# # 2) READ: print file content to stdout
# $EXEC $FS write /tmp/foo.txt /foo.txt
# $EXEC $FS ls
# $EXEC $FS read /foo.txt

# # 3) WRITE: Write file and create directories that don't exist
# $EXEC $FS write /tmp/foo.txt /dir1/dir2/foo.txt
# $EXEC $FS ls
# $EXEC $FS read /dir1/dir2/foo.txt

# # 4) REMOVE: Remove file/directory
# $EXEC $FS write /tmp/foo.txt /dir1/dir2/foo.txt
# $EXEC $FS remove /foo.txt               # FAIL
# $EXEC $FS remove /dir1                  # FAIL
# $EXEC $FS remove /dir1/dir2             # FAIL

# $EXEC $FS remove /dir1/dir2/foo.txt     # PASS
# $EXEC $FS remove /dir1/dir2             # PASS
# $EXEC $FS remove /dir1                  # PASS
# $EXEC $FS ls

# !!! TODO: Fix directory !!! 
$EXEC $FS write /tmp/foo.txt /dir1/foo.txt
$EXEC $FS write /tmp/foo.txt /dir2/foo.txt

$EXEC $FS ls

$EXEC $FS remove /dir1/foo.txt
$EXEC $FS remove /dir2/foo.txt

$EXEC $FS remove /dir1
$EXEC $FS ls            # ERROR: dir2 should still exist instead of dir1