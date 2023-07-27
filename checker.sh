bash run.sh $1
diff -Z stage1.txt $1/stage1.txt 
echo "stage 1 done"
diff -Z stage2.txt $1/stage2.txt 
echo "stage 2 done"
diff -Z stage3.txt $1/stage3.txt 
echo "stage 3 done"
