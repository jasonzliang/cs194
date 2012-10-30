#! /bin/sh

JAVA_SOL=java_sol

echo "COMPILING JAVA SOURCE"
javac src/Driver.java -sourcepath src/ -d .
echo "RUNNING JAVA SOLVER"
java Driver $JAVA_SOL
echo "RUNNING PYTHON SOLVER"
python pgsolver_new.py
echo "COMPARING OUTPUT"
$JAVA_SOL=$JAVA_SOL".txt.mtx"
python test_driver.py $JAVA_SOL output.txt.mtx  

