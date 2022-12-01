# Exit when any command fails
set -e

# Go to current working directory
cd "$PWD"

# Variables
EXEC="./build/vmhFS"
FS="/tmp/tmpFS"

# Unit testing
echo "vmhFS Unit Testing"
echo "------------------------------------"

# Compile
echo "Compiling..."
make
echo "Compilation successful"
echo "------------------------------------"

# 0) WRITE:Create a 10 MB file system
$EXEC $FS create 10
echo "------------------------------------"

# Generating file
echo "Generating file for testing..."
dd if=/dev/zero of=/tmp/4mb bs=1M count=4
echo "Random string 123" > /tmp/foo.txt
echo "------------------------------------"


# PREDEFINED TESTS (please follow the order)
# Uncomment one test by one test

# 1) WRITE: Write file and create directories that don't exist + Overwrite old file
$EXEC $FS write /tmp/4mb /dir1/dir2/4mb1
$EXEC $FS write /tmp/4mb /dir1/dir2/4mb2
$EXEC $FS write /tmp/4mb /dir1/dir2/4mb3
$EXEC $FS ls / -r

# # 2) READ: print file content to stdout
# $EXEC $FS write /tmp/foo.txt /dir1/dir2/foo.txt
# $EXEC $FS read /dir1/dir2/foo.txt
# $EXEC $FS read /dir1/dir2/foo2.txt

# # 3) REMOVE: Remove file/directory
# $EXEC $FS write /tmp/foo.txt /dir1/dir2/foo.txt
# $EXEC $FS remove /dir1/dir2/foo.txt     # PASS
# $EXEC $FS remove /dir1/dir2             # PASS
# $EXEC $FS remove /dir1                  # PASS
# $EXEC $FS ls / -r
# $EXEC $FS remove /

# # 4) LS: recursive and not recursive
# $EXEC $FS write /tmp/foo.txt /dir1/dir2/dir3/dir4/dir5/foo.txt
# $EXEC $FS write /tmp/foo.txt /dir1/foo1.txt
# $EXEC $FS write /tmp/foo.txt /dir1/foo2.txt
# $EXEC $FS write /tmp/foo.txt /dir1/foo3.txt
# $EXEC $FS ls /dir1
# $EXEC $FS ls /dir1 -r

# # 5) SIZE
# $EXEC $FS write /tmp/foo.txt /dir1/foo1.txt
# $EXEC $FS write /tmp/4mb /dir2/4mb1
# $EXEC $FS size /dir1 -b -r
# $EXEC $FS size /dir2 -k -r
# $EXEC $FS size / -b -stat