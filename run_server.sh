if [ "$1" = "build" ];then
./configure  --prefix=/home/learning/nginx_learn
make
fi
gdbserver localhost:2333 ./objs/nginx