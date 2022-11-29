# Exit when any command fails
set -e

# Go to current working directory
cd "$PWD"

# Variables
EXEC="./build/vhmFS"
FS="/tmp/tmpFS"

# Unit testing
echo "vhmFS Unit Testing"
echo "------------------------------------"

# Compile
echo "Compiling..."
make
echo "Compilation successful"
echo "------------------------------------"

# Create a 10 MB file system
$EXEC $FS create 10
echo "------------------------------------"

# Generating file
echo "Generating file for testing..."
dd if=/dev/zero of=/tmp/6mb bs=1M count=6
echo "Random string 123" > /tmp/foo.txt
echo "------------------------------------"


# PREDEFINED TESTS (please follow the order)

# # 1) WRITE: Overwrite old file
# $EXEC $FS write /tmp/foo.txt /foo.txt
# $EXEC $FS write /tmp/6mb /6mb1
# $EXEC $FS ls / -r
# $EXEC $FS write /tmp/6mb /6mb2
# $EXEC $FS ls / -r

# # 2) READ: print file content to stdout
# $EXEC $FS write /tmp/foo.txt /foo.txt
# $EXEC $FS ls / -r
# $EXEC $FS read /foo.txt

# # 3) WRITE: Write file and create directories that don't exist
# $EXEC $FS write /tmp/foo.txt /dir1/dir2/foo.txt
# $EXEC $FS ls / -r
# $EXEC $FS read /dir1/dir2/foo.txt

# # 4) REMOVE: Remove file/directory
# $EXEC $FS write /tmp/foo.txt /dir1/dir2/foo.txt
# $EXEC $FS remove /foo.txt               # FAIL
# $EXEC $FS remove /dir1                  # FAIL
# $EXEC $FS remove /dir1/dir2             # FAIL

# $EXEC $FS remove /dir1/dir2/foo.txt     # PASS
# $EXEC $FS remove /dir1/dir2             # PASS
# $EXEC $FS remove /dir1                  # PASS
# $EXEC $FS ls / -r

# # 5) LS: recursive and not recursive
# $EXEC $FS write /tmp/foo.txt /dir1/dir2/dir3/dir4/dir5/foo.txt
# $EXEC $FS write /tmp/foo.txt /dir1/foo1.txt
# $EXEC $FS write /tmp/foo.txt /dir1/foo2.txt
# $EXEC $FS write /tmp/foo.txt /dir1/foo3.txt
# $EXEC $FS ls /dir1
# $EXEC $FS ls /dir1 -r

# # 6) SIZE
# $EXEC $FS write /tmp/foo.txt /dir1/foo1.txt
# $EXEC $FS write /tmp/foo.txt /dir1/dir2/foo2.txt

# $EXEC $FS write /tmp/6mb /dir2/6mb1

# $EXEC $FS size /dir1 -b -r
# $EXEC $FS size /dir2 -k -r