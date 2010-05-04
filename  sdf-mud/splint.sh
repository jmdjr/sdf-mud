a=*.c
b="${a/ sha256.c /i }"
echo $b

for f in *.c
do
splint $f
echo "press key to continue"
read var1
done
