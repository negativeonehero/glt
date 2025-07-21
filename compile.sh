gcc main.c $(find {gl,glx,gles,util} -name "*.c") -Iinclude -Igles -Igl/shader -Iutil -shared -fPIC -o libglt.so -O2 -g
